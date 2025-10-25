                           __
        ____ _      ______/ /
       / __ \ | /| / / __  / 
      / /_/ / |/ |/ / /_/ /  
     / .___/|__/|__/\__,_/   
    /_/
    [print working directory]

Application (dot command) to print the current working directory ZX on Spectrum Next.
For anyone (like me) who keeps forgetting that "CD" does exactly the same thing ...

Development is done with "z88dk" and "Visual Studio Code" on Windows.

--- 
### USAGE

![help.bmp](https://github.com/essszettt/pwd/blob/main/doc/help.bmp)

---
### EXAMPLES

![help.bmp](https://github.com/essszettt/pwd/blob/main/doc/example.bmp)

---
### HISTORY

    0.1.0    Initial release
    0.1.1    Error handling of esxDOS function call improved
    0.1.2    Normalization of the printed path reworked [no trailing '/' anymore]
    0.1.3    Potential error in use of "strupr" fixed
