
/****************/
_magic_I0_ = MAGIC_WORD_8;
_magic_I1_ = MAGIC_WORD_9;

_B_org_  = ORIGIN(BOOTSTRAP);
_B_size_ = BOOTSTRAP_SIZE;

_S_size_ = 2*PAGE_SIZE;
_A_size_ = 84*PAGE_SIZE;
_I0_size_ = _A_size_;
_I1_size_ = _A_size_;

_A_org_  = ORIGIN(USER_PART);
_S_org_  = _A_org_ + _A_size_;
_I0_org_  = _S_org_ + _S_size_;
_I1_org_  = _I0_org_ + _I0_size_; 

/****************/
/* Sanity check */
ALL_P_SIZE = _B_size_ + _A_size_ + _S_size_ + _I0_size_ + _I1_size_;
ASSERT(FLASH_SIZE >= ALL_P_SIZE, "!!! Out of FLASH !!!");

B_ALIGN = _B_org_ % PAGE_SIZE;
ASSERT( B_ALIGN == 0, "!!! Bootstrap is not aligned on PAGE !!!");
B_ALIGN = _B_size_ % PAGE_SIZE;
ASSERT( B_ALIGN == 0, "!!! Bootstrap is not aligned on PAGE !!!");

A_ALIGN = _A_org_ % PAGE_SIZE;
ASSERT( A_ALIGN == 0, "!!! Active part is not aligned on PAGE !!!");
A_ALIGN = _A_size_ % PAGE_SIZE;
ASSERT( A_ALIGN == 0, "!!! Active part is not aligned on PAGE !!!");

S_ALIGN = _S_org_ % PAGE_SIZE;
ASSERT( S_ALIGN == 0, "!!! Storage part is not aligned on PAGE !!!");
S_ALIGN = _S_size_ % PAGE_SIZE;
ASSERT( S_ALIGN == 0, "!!!  part is not aligned on PAGE !!!");

I0_ALIGN = _I0_org_ % PAGE_SIZE;
ASSERT( I0_ALIGN == 0, "!!! Inactive part 0 is not aligned on PAGE !!!");
I0_ALIGN = _I0_size_ % PAGE_SIZE;
ASSERT( I0_ALIGN == 0, "!!! Inactive part 0 is not aligned on PAGE !!!");

I1_ALIGN = _I1_org_ % PAGE_SIZE;
ASSERT( I1_ALIGN == 0, "!!! Inactive part 1 is not aligned on PAGE !!!");
I1_ALIGN = _I1_size_ % PAGE_SIZE;
ASSERT( I1_ALIGN == 0, "!!! Inactive part 1 is not aligned on PAGE !!!");
