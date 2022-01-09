#ifdef __WATCOMC__
/* __TURBOC__       __WATCOMC__ */   
#define malloc      _fmalloc     
#define calloc      _fcalloc     
#define free        _ffree       
#define realloc     _frealloc    
#define strupr      _strupr   
#define disable     _disable
#define enable      _enable
#define getvect     _dos_getvect
#define setvect     _dos_setvect
#define inportb     inp
#define inport      inpw
#define outportb    outp
#define outport     outpw
#define farmalloc   _fmalloc
#define farfree     _ffree
#define asm         _asm
#define NULL        ((void *)0)
#endif
