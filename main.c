#include <stdio.h>
#include <IOKit/hid/IOHIDManager.h>
#include <AudioToolbox/AudioToolbox.h>

SystemSoundID soundID;

void handle_keyboard_event(void *context, IOReturn result, void *sender, IOHIDValueRef value);
void load_keyboard_sound();
void play_keyboard_sound();

int main()
{
    /*
        Init IOHID for keyboard events
    */
    // Create the IOHIDManager
    IOHIDManagerRef manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if (!manager) 
    {
      fprintf(stderr, "Failed to create IOHIDManager\n");
      return 1;
    }

    // Set up device matching criteria for keyboards
    CFMutableDictionaryRef keyboardMatch = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFDictionarySetValue(keyboardMatch, CFSTR(kIOHIDDeviceUsagePageKey), CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &(int){kHIDPage_GenericDesktop}));
    CFDictionarySetValue(keyboardMatch, CFSTR(kIOHIDDeviceUsageKey), CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &(int){kHIDUsage_GD_Keyboard}));

    IOHIDManagerSetDeviceMatching(manager, keyboardMatch);

    CFRelease(keyboardMatch);// Retrieve set of devices managed

    // Register callback function for keyboard events
    IOHIDManagerRegisterInputValueCallback(manager, handle_keyboard_event, NULL);

    // Schedule with run loop
    IOHIDManagerScheduleWithRunLoop(manager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

    // Open manager
    IOReturn ret = IOHIDManagerOpen(manager, kIOHIDOptionsTypeNone);
    if (ret != kIOReturnSuccess) 
    {
      fprintf(stderr, "Failed to open manager\n");
      CFRelease(manager);
      return 1;
    }

    /*
        Init soundID and load sound file  
    */
    load_keyboard_sound();

    // Run the run loop
    CFRunLoopRun();

    // Clean up
    IOHIDManagerClose(manager, kIOHIDOptionsTypeNone);
    CFRelease(manager);
    AudioServicesDisposeSystemSoundID(soundID);

    return 0;
}

void handle_keyboard_event(void *context, IOReturn result, void *sender, IOHIDValueRef value) 
{
    IOHIDElementRef element = IOHIDValueGetElement(value);
    uint32_t value_int = IOHIDValueGetIntegerValue(value);
    uint32_t usage_page = IOHIDElementGetUsagePage(element);
    uint32_t usage = IOHIDElementGetUsage(element);

    // Check if the event is a key press
    if (usage_page == kHIDPage_KeyboardOrKeypad && value_int == 1)
    {
        if(usage == -1)
        {
            return;
        }

        // Play Sound
        play_keyboard_sound();

        char key = '\0';
        switch(usage)
        {
            case 20:
                key = 'q'; 
                break;
            case 26:
                key = 'w';
                break;
            case 8:
                key = 'e';
                break;
            case 21:
                key = 'r';
                break;
            case 23:
                key = 't';
                break;
            case 28:
                key = 'y';
                break;
            case 24:
                key = 'u';
                break;
            case 12:
                key = 'i';
                break;
            case 18:
                key = 'o';
                break;
            case 19:
                key = 'p';
                break;
            case 4:
                key = 'a';
                break;
            case 22:
                key = 's';
                break;
            case 7:
                key = 'd';
                break;
            case 9:
                key = 'f';
                break;
            case 10:
                key = 'g';
                break;
            case 11:
                key = 'h';
                break;
            case 13:
                key = 'j';
                break;
            case 14:
                key = 'k';
                break;
            case 15:
                key = 'l';
                break;
            case 29:
                key = 'z';
                break;
            case 27:
                key = 'x';
                break;
            case 6:
                key = 'c';
                break;
            case 25:
                key = 'v';
                break;
            case 5:
                key = 'b';
                break;
            case 17:
                key = 'n';
                break;
            case 16:
                key = 'm';
                break;
            default:
                printf("Unknown Key: %d\n", usage);
                break;
        }

        printf("key: %c\n", key);
    }
}

void load_keyboard_sound()
{
    CFURLRef soundURL = CFURLCreateWithFileSystemPath(NULL, CFSTR("./analog-appliance-button-6.mp3"), kCFURLPOSIXPathStyle, false);

    AudioServicesCreateSystemSoundID(soundURL, &soundID);

    CFRelease(soundURL);
    
}

void play_keyboard_sound()
{
    printf("Playing Sound");
    AudioServicesPlaySystemSound(soundID);
}

