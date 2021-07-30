
/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 * 
 */ 

#ifndef DEFINE_WIT_H
#define DEFINE_WIT_H

namespace OT {
	enum  {
		Variable = 1<<0,
		Typedef = 1<<1,
		Extern = 1<<2,
		Function = 1<<3,
		Struct = 1<<4,
		Operator = 1<<5,
		Macro = 1<<6,
		Call = 1<<7,
		Define = 1<<8,
		Class = 1<<9,
		Namespace = 1<<10,
		Enum = 1<<11,
		Module = 1<<12,
		Package = 1<<13,
		Other = 1<<30
	};
};

#endif
