#ifndef PATOMIC_STATIC_ASSERT

#define PATOMIC_STATIC_ASSERT(m, e) typedef char _patomic_sa_##m[(e)?1:-1]

#endif  /* !PATOMIC_STATIC_ASSERT */
