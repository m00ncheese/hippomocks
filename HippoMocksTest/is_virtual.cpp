#include "Framework.h"
#include <iostream>
#include "hippomocks.h"

class IL { 
public:
	void f() { std::cout << "0" << std::endl; }
	virtual void g() { std::cout << "1" << std::endl; }
	virtual void h() { std::cout << "2" << std::endl; }
};

class SecondBase
{
public:
	virtual void k() { std::cout << "3" << std::endl; }
	int x;
};

class ThirdBase
{
public:
	virtual void l() { std::cout << "4" << std::endl; }
	virtual void m() { std::cout << "4" << std::endl; }
};

class ILD : public IL, public SecondBase, public ThirdBase
{
};

TEST(checkNonVirtual) 
{
	EQUALS(HippoMocks::virtual_index(&ILD::f).first, -1);
}

TEST(checkFirstVirtual) 
{
	EQUALS(HippoMocks::virtual_index(&ILD::g).first, 0);
	EQUALS(HippoMocks::virtual_index(&ILD::g).second, 0 + FUNCTION_BASE);
}

TEST(checkSecondVirtual) 
{
	EQUALS(HippoMocks::virtual_index(&ILD::h).first, 0);
	EQUALS(HippoMocks::virtual_index(&ILD::h).second, 1 + FUNCTION_BASE);
}

TEST(checkSecondBaseFirstVirtual) 
{
	EQUALS(HippoMocks::virtual_index((void (ILD::*)())&ILD::k).first, 1);
	EQUALS(HippoMocks::virtual_index((void (ILD::*)())&ILD::k).second, 0 + FUNCTION_BASE);
}

TEST(checkThirdBaseSecondVirtualAfterInt) 
{
	EQUALS(HippoMocks::virtual_index((void (ILD::*)())&ILD::m).first, 3);
	EQUALS(HippoMocks::virtual_index((void (ILD::*)())&ILD::m).second, 1 + FUNCTION_BASE);
}

using HippoMocks::func_index;

TEST (checkVirtualIndexGreater64_BecauseLengthOfOpcodeGrows)
{
    EQUALS(0, HippoMocks::virtual_index((int (func_index::*)())&func_index::f66).first);
    EQUALS(66 + FUNCTION_BASE, HippoMocks::virtual_index((int (func_index::*)())&func_index::f66).second);
}

TEST(checkPointerConversionIsOk) 
{
	void (ThirdBase::*f)() = &ThirdBase::m;
	EQUALS(HippoMocks::virtual_index((void (ILD::*)())f).first, 3);
	EQUALS(HippoMocks::virtual_index((void (ILD::*)())f).second, 1 + FUNCTION_BASE);
}

