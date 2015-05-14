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

void SaveImage( CGImageRef image, CFURLRef outputFile ){

  CGImageDestinationRef output = CGImageDestinationCreateWithURL( outputFile,
                                                                  kPNG,
                                                                  1,
                                                                  NULL );

  CGImageDestinationAddImage( output,
                              image,
                              NULL );

  CGImageDestinationFinalize( output );

}

void GrabScreen( char *outputFileName ){
  // Phase 0, just the first screen: Done
  // Phase 1, all screens, different files: Done
  // Phase 2, Joined images
  // Phase 3, handle errors?

  // Get the list of displays
  const uint32_t kMaxDisplays = 32;
  uint32_t displayCount = 0;
  CGDirectDisplayID *displays = static_cast<CGDirectDisplayID *>( malloc( kMaxDisplays * sizeof( CGDirectDisplayID ) ) );
  CGError error = CGGetActiveDisplayList( kMaxDisplays, displays, &displayCount );
  

  size_t stringLen = strlen( outputFileName ) + 20;
  char *tmpStringBuf = static_cast<char *>( malloc( stringLen ) );

  for (uint32_t i = 0; i < displayCount; i++){
    snprintf( tmpStringBuf, stringLen-1, "file://%s-%02d.png", outputFileName, i );
    CFURLRef outputFile = CreateURLFromCString( tmpStringBuf );
    CGImageRef image = CGDisplayCreateImage( displays[i] );
    SaveImage( image, outputFile );
    CFRelease( outputFile );
  }

}


// TODO(brian): Handle me some errors
int main(int argc, char **argv){
  std::cout << "Writing screenshot to " << argv[1] << "...";
  GrabScreen( argv[1] );
  std::cout << "...Complete." << std::endl;
  return 0;
}
