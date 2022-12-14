/*********************/
/* par.c             */
/* for Par 3.20      */
/* Copyright 1993 by */
/* Adam M. Costello  */
/*********************/

/* This is ANSI C code. */

#include <getopt.h>
#include "errmsg.h"
#include "buffer.h"    /* Also includes <stddef.h>. */
#include "reformat.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#undef NULL
#define NULL ((void *) 0)


const char * const progname = "par";
const char * const version = "3.20";


static int digtoint(char c)

/* Returns the value represented by the digit c,   */
/* or -1 if c is not a digit. Does not use errmsg. */
{
  return c == '0' ? 0 :
         c == '1' ? 1 :
         c == '2' ? 2 :
         c == '3' ? 3 :
         c == '4' ? 4 :
         c == '5' ? 5 :
         c == '6' ? 6 :
         c == '7' ? 7 :
         c == '8' ? 8 :
         c == '9' ? 9 :
         -1;

  /* We can't simply return c - '0' because this is ANSI  */
  /* C code, so it has to work for any character set, not */
  /* just ones which put the digits together in order.    */
}


static int strtoudec(const char *s, int *pn)

/* Puts the decimal value of the string s into *pn, returning */
/* 1 on success. If s is empty, or contains non-digits,       */
/* or represents an integer greater than 9999, then *pn       */
/* is not changed and 0 is returned. Does not use errmsg.     */
{
  int n = 0;

  if (!*s) return 0;

  do {
    if (n >= 1000 || !isdigit(*s)) return 0;
    n = 10 * n + digtoint(*s);
  } while (*++s);

  *pn = n;

  return 1;
}

 static void parseopt(
   int argc, char * const *argv, const char *opt, int *pwidth, int *pprefix,
   int *psuffix, int *phang, int *plast, int *pmin
 )
 {
    
      static struct option long_options[] = {
          {"width", required_argument, 0,  'w' },
          {"version", no_argument, 0, 'v' }, //not short arguements only long
          {"prefix", required_argument, 0, 'p' },
          {"suffix", required_argument, 0, 's' },
          {"hang", optional_argument, 0, 'h' },
          {"last", optional_argument, 0, 'x' }, 
          {"min", optional_argument, 0, 'y' },
          {"no-last", 0 , 0 , 'n'}, //not short arguements only long
          {"no-min", 0, 0, 'z'}, //not short arguements only long
          {0, 0, 0, 0 }
        };
    
    int c = 0;

    //sorts the -arguements and then goes through them and check if their valid or not and
    //returns a ? 

    while (c != -1 ) { //goes to the last arguement is reached
      //the value of required_arguement is stored in optarg
      //optind is the index that you are at inside argv

      int c = getopt_long(argc, argv, "w:p:s:h::l::m::", long_options, NULL);
      
      if (c == -1){
         break;
      }

      int variable = 0;

      switch (c)
        {
        case 'w':;
          variable = 0;

          if (strtoudec(optarg,&variable) == 0){
            exit(0);
          }else{
            *pwidth = variable;
             break;
          }

        case 'v':
          printf("%s %s\n", progname, version);
          exit(EXIT_SUCCESS); //exits

        case 'p':
            variable = 0;
            if (strtoudec(optarg,&variable) == 0){
              exit(0);
            }else{
              *pprefix = variable;
              break;
            }

        case 's':
            variable = 0;
            if (strtoudec(optarg,&variable) == 0){
              exit(0);
            }else{
              *psuffix = variable;
              break;
            }
        case 'h': //for hang
            variable = 0;
            if(optarg){
              if (strtoudec(optarg,&variable) == 1){
                if(variable == 1 || variable == 0){
                *phang = variable;
               }
             } else{
                exit(0);
              }
            }else{
              *phang = 1;
              break;
            }

        case 'x': //for last
            *plast = 1;
            break;
        
        case 'y': //for min
           *pmin = 1;
           break;
        
        case 'n':
            *plast = 0; //for no last
             break;

        case 'z': //for no min
          *pmin = 0;
           break;

        default:
          goto badopt;
        }
    }
    //here OPTIND points to the first arguement that is not a option

   int forwidth = -10;
   int forprefix = 10;

   int finalwidth = -10;
   int finalprefix = 10;

   int i = optind;
   
   for (i=optind; i < argc; i++){
      if((strtoudec(argv[i], &forwidth))){ //for width
         if((forwidth > 9) || (forwidth == 9)){ 
          finalwidth = forwidth; //sets it
         }
      }

      if((strtoudec(argv[i], &forprefix))){ //for prefix
        if((forprefix < 8) || (forprefix == 8)){
          finalprefix = forprefix;
        }
          
      }
    }
    //Now we only set it after the for loop
    if(finalwidth >= 9){
      *pwidth = finalwidth;
    }
    if(finalprefix <= 8){
      *pprefix = finalprefix;
    }
    return;

     badopt:
      set_error("Incorrect option has been entered here");
   
 }

// /* comment out
// static void parseopt(
//   const char *opt, int *pwidth, int *pprefix,
//   int *psuffix, int *phang, int *plast, int *pmin
// )
// /* comment out

// /* Parses the single option in opt, setting *pwidth, *pprefix,     */
// /* *psuffix, *phang, *plast, or *pmin as appropriate. Uses errmsg. */

// {
//   const char *saveopt = opt;
//   char oc;
//   int n, r;

//   if (*opt == '-') ++opt;

//   if (!strcmp(opt, "version")) {
//     sprintf(errmsg, "%s %s\n", progname, version);
//     return;
//   }

//   oc = *opt;

//   if (isdigit(oc)) {
//     if (!strtoudec(opt, &n)) goto badopt;
//     if (n <= 8) *pprefix = n;
//     else *pwidth = n;
//   }
//   else {
//     if (!oc) goto badopt;
//     n = 1;
//     r = strtoudec(opt + 1, &n);
//     if (opt[1] && !r) goto badopt;

//     if (oc == 'w' || oc == 'p' || oc == 's') {
//       if (!r) goto badopt;
//       if      (oc == 'w') *pwidth  = n;
//       else if (oc == 'p') *pprefix = n;
//       else                *psuffix = n;
//     }
//     else if (oc == 'h') *phang = n;
//     else if (n <= 1) {
//       if      (oc == 'l') *plast = n;
//       else if (oc == 'm') *pmin = n;
//     }
//     else goto badopt;
//   }

//   *errmsg = '\0';
//   return;

// badopt:
//   sprintf(errmsg, "Bad option: %.149s\n", saveopt);
// }


static char **readlines(void)

/* Reads lines from stdin until EOF, or until a blank line is encountered, */
/* in which case the newline is pushed back onto the input stream. Returns */
/* a NULL-terminated array of pointers to individual lines, stripped of    */
/* their newline characters. Uses errmsg, and returns NULL on failure.     */
{
  struct buffer *cbuf = NULL, *pbuf = NULL;
  int c, blank;
  char ch, *ln, *nullline = NULL, nullchar = '\0', **lines = NULL;

  cbuf = newbuffer(sizeof (char));
  if (is_error()) goto rlcleanup;
  pbuf = newbuffer(sizeof (char *));
  if (is_error()) goto rlcleanup;

  for (blank = 1;  ; ) {
    c = getchar();
    if (c == EOF) break;
    if (c == '\n') {
      if (blank) {
        ungetc(c,stdin);
        break;
      }
      additem(cbuf, &nullchar);
      if (is_error()) goto rlcleanup;
      ln = copyitems(cbuf);
      if (is_error()) goto rlcleanup;
      additem(pbuf, &ln);
      if (is_error()) goto rlcleanup;
      clearbuffer(cbuf);
      blank = 1;
    }
    else {
      if (!isspace(c)) blank = 0;
      ch = c;
      additem(cbuf, &ch);
      if (is_error()) goto rlcleanup;
    }
  }

  if (!blank) {
    additem(cbuf, &nullchar);
    if (is_error()) goto rlcleanup;
    ln = copyitems(cbuf);
    if (is_error()) goto rlcleanup;
    additem(pbuf, &ln);
    if (is_error()) goto rlcleanup;
  }

  additem(pbuf, &nullline);
  if (is_error()) goto rlcleanup;
  lines = copyitems(pbuf);

rlcleanup:

  if (cbuf) freebuffer(cbuf); //frees cbuf
  if (pbuf) {

    if (!lines){
      for (;;) {
        lines = nextitem(pbuf);
        if (!lines) break;
        free(*lines);
      }
    }
    freebuffer(pbuf); //takes in pbuf and frees it
  }

  return lines;
}


static void setdefaults(
  const char * const *inlines, int *pwidth, int *pprefix,
  int *psuffix, int *phang, int *plast, int *pmin
)
/* If any of *pwidth, *pprefix, *psuffix, *phang, *plast, *pmin are     */
/* less than 0, sets them to default values based on inlines, according */
/* to "par.doc". Does not use errmsg because it always succeeds.        */
{
  int numlines;
  const char *start, *end, * const *line, *p1, *p2;

  if (*pwidth < 0) *pwidth = 72;
  if (*phang < 0) *phang = 0;
  if (*plast < 0) *plast = 0;
  if (*pmin < 0) *pmin = *plast;

  for (line = inlines;  *line;  ++line);
  numlines = line - inlines;

  if (*pprefix < 0){
    if (numlines <= *phang + 1)
      *pprefix = 0;
    else {
      start = inlines[*phang];
      for (end = start;  *end;  ++end);
      for (line = inlines + *phang + 1;  *line;  ++line) {
        for (p1 = start, p2 = *line;  p1 < end && *p1 == *p2;  ++p1, ++p2);
        end = p1;
      }
      *pprefix = end - start;
    }
  }

  if (*psuffix < 0){
    if (numlines <= 1)
      *psuffix = 0;
    else {
      start = *inlines;
      for (end = start;  *end;  ++end);
      for (line = inlines + 1;  *line;  ++line) {
        for (p2 = *line;  *p2;  ++p2);
        for (p1 = end;
             p1 > start && p2 > *line && p1[-1] == p2[-1];
             --p1, --p2);
        start = p1;
      }
      while (end - start >= 2 && isspace(*start) && isspace(start[1])) ++start;
      *psuffix = end - start;
    }
 }
}

static void freelines(char **lines)
/* Frees the strings pointed to in the NULL-terminated array lines, then */
/* frees the array. Does not use errmsg because it always succeeds.      */
{
  int index = 0;

  while(*(index + lines) != NULL){ //frees strings until null hit
      free(*(index + lines)); //frees 1 String at a time

      index = index + 1;
  }

  free(lines); //then it frees the array
}


int original_main(int argc, char * const *argv)
{
  int width, widthbak = -1, prefix, prefixbak = -1, suffix, suffixbak = -1,
      hang, hangbak = -1, last, lastbak = -1, min, minbak = -1, c;
  char *parinit, *picopy = NULL, *opt, **inlines = NULL, **outlines = NULL,
       **line;
  const char * const whitechars = " \f\n\r\t\v";

  parinit = getenv("PARINIT");
  if (parinit) {
    picopy = malloc((strlen(parinit) + 1) * sizeof (char));
    if (!picopy) {
      set_error((char *)outofmem);
      goto parcleanup;
    }
    strcpy(picopy,parinit);
    opt = strtok(picopy,whitechars);
    

    parseopt(argc,argv, opt, &widthbak, &prefixbak,
               &suffixbak, &hangbak, &lastbak, &minbak);
      if (is_error()) goto parcleanup;
    
    /*
    while (opt) {
      parseopt(opt, &widthbak, &prefixbak,
               &suffixbak, &hangbak, &lastbak, &minbak);
      if (*errmsg) goto parcleanup;
      opt = strtok(NULL,whitechars);
    }
    free(picopy);
    picopy = NULL;
  */
  }
   
    parseopt(argc,argv, opt, &widthbak, &prefixbak,
               &suffixbak, &hangbak, &lastbak, &minbak);
    if (is_error()) goto parcleanup;

 /* while (*++argv) { 
  }
*/

  for (;;) {
    for (;;) {
      c = getchar();
      if (c != '\n') break;
      putchar(c);
    }
    ungetc(c,stdin);

    inlines = readlines();
    if (is_error()) goto parcleanup;
    if (!*inlines) {
      free(inlines);
      inlines = NULL;
      break;      
    }

    width = widthbak;  prefix = prefixbak;  suffix = suffixbak;
    hang = hangbak;  last = lastbak;  min = minbak;
    setdefaults((const char * const *) inlines,
                &width, &prefix, &suffix, &hang, &last, &min);

    outlines = reformat((const char * const *) inlines,
                        width, prefix, suffix, hang, last, min);
    if (is_error()) goto parcleanup;

    freelines(inlines);
    inlines = NULL;

    for (line = outlines;  *line;  ++line)
      puts(*line);

    freelines(outlines);
    outlines = NULL;
  }

parcleanup:

  if (picopy) free(picopy);
  if (inlines) freelines(inlines);
  if (outlines) freelines(outlines);

  if (is_error()) {
    report_error(stderr); //pass in report stderr
    clear_error(); //so no mem leaks happen
    
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
