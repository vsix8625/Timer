# Timer CLI Tool  

Timer is a versatile countdown timer implemented in C, offering a simple yet effective terminal interface.  
Users can define hours, minutes, and seconds for a countdown using intuitive command-line options.  

## Features  

- **Flexible Countdown Options**: Specify countdown durations in hours, minutes, seconds, or a combination of all three.  
- **Real-Time Updates**: Displays a dynamic countdown timer in the terminal with color-coded formatting.  
- **Input Validation**: Ensures entered values are within valid ranges:  
  - Maximum hours: `120`  
  - Maximum minutes: `59`  
  - Maximum seconds: `59`  
- **Error Handling**: Detects and reports invalid input values or missing arguments.  
- **Default Shell Integration**: Uses system commands like `date` and `clear` for user-friendly interaction.  

---

## Usage  

### Command-line Options  

```bash  
Usage: Timer [opt1] <value> [opt2] <value> ...  

Options:  
  -h, --hour <value>   Countdown hours (0-120)  
  -m, --minute <value> Countdown minutes (0-59)  
  -s, --second <value> Countdown seconds (0-59)  
```

Usage example:  
 - Timer -s 10  | counts for 10 seconds.   
 - Timer -s 20 -m 1 -h 1  | counts for 1 hour, 1 minute and 20 seconds.  
 - Timer -h 1 -m 1 -s 20  | the order of opts does not matter.  
 - Timer -h 12 -a 13 -m 1 -s 20  | -a and 13 that comes after it, are invalid options and will be ignored.  
 - Timer -m 1 -s 20  | counts for 1 minute and 20 seconds, the hour missing option is defaulted to 0.  

## Note
   Ctr-C to exit before countdown ends.  
