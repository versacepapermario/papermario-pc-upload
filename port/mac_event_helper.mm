// mac_event_helper.mm - macOS-specific event priming
// On macOS, the first SDL_PumpEvents call blocks in the Cocoa event loop
// waiting for a native event. This helper posts a dummy NSEvent to wake it up.

#ifdef __APPLE__
#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#import <Metal/Metal.h>
#import <ImageIO/ImageIO.h>
#include <dlfcn.h>

extern "C" void mac_prime_event_loop(void) {
    @autoreleasepool {
        // Post a dummy application-defined event to wake up the Cocoa event loop
        NSEvent *event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                            location:NSMakePoint(0, 0)
                                       modifierFlags:0
                                           timestamp:0
                                        windowNumber:0
                                             context:nil
                                             subtype:0
                                               data1:0
                                               data2:0];
        [NSApp postEvent:event atStart:YES];
    }
}

extern "C" int mac_capture_window_screenshot(const char* outputPath) {
    @autoreleasepool {
        // Find the main window
        NSArray<NSWindow*> *windows = [NSApp windows];
        if ([windows count] == 0) {
            fprintf(stderr, "[screenshot] No windows found\n");
            return -1;
        }

        NSWindow *mainWindow = nil;
        for (NSWindow *w in windows) {
            if ([w isVisible] && [w contentView] != nil) {
                mainWindow = w;
                break;
            }
        }
        if (!mainWindow) {
            fprintf(stderr, "[screenshot] No visible window found\n");
            return -1;
        }

        CGWindowID windowID = (CGWindowID)[mainWindow windowNumber];
        fprintf(stderr, "[screenshot] Capturing window ID %d\n", windowID);

        // Dynamically load CGWindowListCreateImage (marked unavailable in macOS 15 SDK but still works)
        typedef CGImageRef (*CGWindowListCreateImageFunc)(CGRect, uint32_t, uint32_t, uint32_t);
        static CGWindowListCreateImageFunc pCGWindowListCreateImage = nullptr;
        if (!pCGWindowListCreateImage) {
            pCGWindowListCreateImage = (CGWindowListCreateImageFunc)dlsym(RTLD_DEFAULT, "CGWindowListCreateImage");
        }

        CGImageRef image = nullptr;
        if (pCGWindowListCreateImage) {
            image = pCGWindowListCreateImage(
                CGRectNull,
                kCGWindowListOptionIncludingWindow,
                windowID,
                kCGWindowImageBoundsIgnoreFraming | kCGWindowImageBestResolution
            );
        }

        if (!image) {
            fprintf(stderr, "[screenshot] CGWindowListCreateImage failed\n");
            return -1;
        }

        int pixelW = (int)CGImageGetWidth(image);
        int pixelH = (int)CGImageGetHeight(image);
        fprintf(stderr, "[screenshot] Captured %dx%d image\n", pixelW, pixelH);

        // Write as PNG using CGImageDestination
        NSString *path = [NSString stringWithUTF8String:outputPath];
        NSURL *url = [NSURL fileURLWithPath:path];
        CGImageDestinationRef dest = CGImageDestinationCreateWithURL(
            (__bridge CFURLRef)url, (__bridge CFStringRef)@"public.png", 1, NULL);

        if (dest) {
            CGImageDestinationAddImage(dest, image, NULL);
            CGImageDestinationFinalize(dest);
            CFRelease(dest);
            fprintf(stderr, "[screenshot] Saved PNG to %s\n", outputPath);
        } else {
            fprintf(stderr, "[screenshot] CGImageDestination failed\n");
            CGImageRelease(image);
            return -1;
        }

        CGImageRelease(image);
        return 0;
    }
}
#endif
