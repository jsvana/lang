#include "atom.h"

#include <stdlib.h>

atom *atom_create() {
	atom *a = malloc(sizeof(atom));
	return a;
}
