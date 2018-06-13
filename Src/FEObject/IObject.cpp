#include "IObject.h"

using namespace std;

unsigned int IObject::_countID = 0;
list<IObject*> IObject::_objList;
list<IObject*> IObject::_dataObjList;