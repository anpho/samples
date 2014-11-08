Samples
=======

I share some sample codes here.

All codes are written by me and are under MIT license by default.

### [passportKeyboardEventWrapper](https://github.com/anpho/samples/blob/master/passportKeyboardEventWrapper.js)

a simple wrapper to convert passport touchable keyboard events to up/down/left/right directions.

Tips:
>What about add some move distance limit?

### [Modify contact info while call incoming](https://github.com/anpho/samples/blob/master/CallerLocation.cpp)

This is a headless service sample, which demostrates how to modify the Contact information by add prefix/surfix to display location info. 

Note:
>When the phone is ringing, onCallUpdated slot is triggered several times, this service modifies the contact information at the first time when phoneNumber is available, and the PhoneUI will update the information at next trigger.

Tips:
>Create a new contact for unknown incoming number, and delete that contact after the phone hanged up.






[Merrick Zhang](http://anpho.github.io)
