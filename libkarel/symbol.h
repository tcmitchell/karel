#ifndef __symbol_h
#define __symbol_h

/*!
 * representation of a symbol
 */
typedef	struct Symbol {
  char *name;
  int addr;
  struct Symbol *next;
} Symbol;

/*!
 * find s in symbol table
 */
extern Symbol *lookup(char *s);

/*!
 * install s in symbol table
 */
extern void install(char *s);

#endif /* __symbol_h */
