##PicoDst QA Maker
RHIC year 2014 Run, with Heavy Flavor Tracker
  
## Declaration
This code was original inherit form next location:
cvs co offline/users/dongx/pico/QA

This code only works for run14 SL16d data set.

If you want to use the code for other data set, you are encouraged to rely on the original version which mentioned above. This one is only for you reference.

Another tips. Since the StPicoDst Maker changes for each data set, remember change them accordingly.
- - -
###How to run this code:  
```bash
starver SL16d
cons

root4star -b -l -q macro/qaPicoDst.C

root4star -b -l -q macro/makeQaPlotsOnPdf.C
```
