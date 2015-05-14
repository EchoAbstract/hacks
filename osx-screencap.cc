// To compile:
//   clang++ -Weverything osx-screencap.cc -o osx-screencap -framework CoreGraphics -framework CoreFoundation -framework ImageIO
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ImageIO/ImageIO.h>
#include <iostream>

const CFStringRef kPNG = CFSTR("public.png");

CFURLRef CreateURLFromCString( char *url ){
  CFStringRef stringRep = CFStringCreateWithCString( kCFAllocatorDefault,
                                                  url,
                                                  kCFStringEncodingMacRoman );
                                                  
  CFURLRef cfurl = CFURLCreateWithString( kCFAllocatorDefault,
                                          stringRep,
                                          NULL );

  CFRelease( stringRep );

  return cfurl;
}

void GrabScreen( char *outputFileName ){
  // Phase 0, just the first screen
  // Phase 1, all screens, different files
  // Phase 2, Joined images
  CFURLRef outputFile = CreateURLFromCString( outputFileName );
  CGImageRef image = CGDisplayCreateImage( kCGDirectMainDisplay );
  CGImageDestinationRef output = CGImageDestinationCreateWithURL( outputFile,
                                                                  kPNG,
                                                                  1,
                                                                  NULL );

  CGImageDestinationAddImage( output,
                              image,
                              NULL );

  CGImageDestinationFinalize( output );

}


// TODO(brian): Handle me some errors
int main(int argc, char **argv){
  std::cout << "Writing screenshot to " << argv[1] << "...";
  GrabScreen( argv[1] );
  std::cout << "...Complete." << std::endl;
  return 0;
}
