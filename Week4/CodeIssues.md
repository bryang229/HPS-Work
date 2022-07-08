# Issues listed out:
- The 8x8 Display has errors when showing time, the time is internally correct however when it comes to displaying issues occur with the binary conversion however it is unclear as when Serialed the values seem to be perfect
- The Rotary Encoder has been render useless with the addition of the 8x8 code, previously the code was function, the menu would scroll and date could be changed however it has issues reacting to change rather then even detecting change. The Interrupt of the Rotary Encoder is still ran yet the updated values are ingored
- In the function used to get the day of the week Monday - Sunday an error occurs causing all saturdays to be given a 0 index when it should be 6
