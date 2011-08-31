#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <google/cmockery.h>

#include "cstl/cstl_def.h"
#include "cstl/cstl_alloc.h"
#include "cstl/cstl_types.h"
#include "cstl/cstl_iterator.h"
#include "cstl/cvector.h"
#include "cstl/cstl_hashtable_iterator.h"
#include "cstl/cstl_hashtable_private.h"
/*#include "cstl/cstl_hashtable.h"*/
#include "cstl_hashtable_aux.h"
#include "cstl/cstring.h"

#include "ut_def.h"
#include "ut_cstl_hashtable_aux.h"

UT_SUIT_DEFINATION(cstl_hashtable_aux, _hashtable_is_created)
/*
 * test _hashtable_is_created
 */
UT_CASE_DEFINATION(_hashtable_is_created)
void test__hashtable_is_created__null_hashtable(void** state)
{
    expect_assert_failure(_hashtable_is_created(NULL));
}

void test__hashtable_is_created__non_inited_allocator(void** state)
{
    _hashtable_t htable;

    type_register(_hashnode_t*, NULL, NULL, NULL, NULL);

    _create_vector_auxiliary(&htable._t_bucket, _HASHTABLE_NODE_NAME);
    htable._t_nodecount = 0;
    htable._t_hash = NULL;
    htable._t_compare = NULL;
    htable._t_typeinfo._t_style = _TYPE_C_BUILTIN;
    htable._t_typeinfo._pt_type = (_type_t*)0x999;
    _alloc_init(&htable._t_allocator);
    htable._t_allocator._t_mempoolsize = 1;
    assert_false(_hashtable_is_created(&htable));
    htable._t_allocator._t_mempoolsize = 0;
    _alloc_destroy(&htable._t_allocator);
}

void test__hashtable_is_created__invalid_rbroot_bucket(void** state)
{
    _hashtable_t htable;

    htable._t_nodecount = 0;
    htable._t_hash = NULL;
    htable._t_compare = NULL;
    htable._t_typeinfo._t_style = _TYPE_C_BUILTIN;
    htable._t_typeinfo._pt_type = (_type_t*)0x999;
    _alloc_init(&htable._t_allocator);
    assert_false(_hashtable_is_created(&htable));
    _alloc_destroy(&htable._t_allocator);
}

void test__hashtable_is_created__invalid_rbroot_hash(void** state)
{
    _hashtable_t htable;

    _create_vector_auxiliary(&htable._t_bucket, _HASHTABLE_NODE_NAME);
    htable._t_hash = (unary_function_t)0xabc;
    htable._t_compare = NULL;
    htable._t_nodecount = 0;
    htable._t_typeinfo._t_style = _TYPE_C_BUILTIN;
    htable._t_typeinfo._pt_type = (_type_t*)0x999;
    _alloc_init(&htable._t_allocator);
    assert_false(_hashtable_is_created(&htable));
    _alloc_destroy(&htable._t_allocator);
}

void test__hashtable_is_created__invalid_compare(void** state)
{
    _hashtable_t htable;

    _create_vector_auxiliary(&htable._t_bucket, _HASHTABLE_NODE_NAME);
    htable._t_hash = NULL;
    htable._t_compare = (binary_function_t)0x888;
    htable._t_nodecount = 0;
    htable._t_typeinfo._t_style = _TYPE_C_BUILTIN;
    htable._t_typeinfo._pt_type = (_type_t*)0x999;
    _alloc_init(&htable._t_allocator);
    assert_false(_hashtable_is_created(&htable));
    _alloc_destroy(&htable._t_allocator);
}

void test__hashtable_is_created__invalid_nodecount(void** state)
{
    _hashtable_t htable;

    _create_vector_auxiliary(&htable._t_bucket, _HASHTABLE_NODE_NAME);
    htable._t_hash = NULL;
    htable._t_compare = NULL;
    htable._t_nodecount = 9;
    htable._t_typeinfo._t_style = _TYPE_C_BUILTIN;
    htable._t_typeinfo._pt_type = (_type_t*)0x999;
    _alloc_init(&htable._t_allocator);
    assert_false(_hashtable_is_created(&htable));
    _alloc_destroy(&htable._t_allocator);
}

void test__hashtable_is_created__invalid_typeinfo_style(void** state)
{
    _hashtable_t htable;

    _create_vector_auxiliary(&htable._t_bucket, _HASHTABLE_NODE_NAME);
    htable._t_hash = NULL;
    htable._t_compare = NULL;
    htable._t_nodecount = 0;
    htable._t_typeinfo._t_style = 90;
    htable._t_typeinfo._pt_type = (_type_t*)0x999;
    _alloc_init(&htable._t_allocator);
    assert_false(_hashtable_is_created(&htable));
    _alloc_destroy(&htable._t_allocator);
}

void test__hashtable_is_created__invalid_typeinfo_type(void** state)
{
    _hashtable_t htable;

    _create_vector_auxiliary(&htable._t_bucket, _HASHTABLE_NODE_NAME);
    htable._t_hash = NULL;
    htable._t_compare = NULL;
    htable._t_nodecount = 0;
    htable._t_typeinfo._t_style = _TYPE_C_BUILTIN;
    htable._t_typeinfo._pt_type = NULL;
    _alloc_init(&htable._t_allocator);
    assert_false(_hashtable_is_created(&htable));
    _alloc_destroy(&htable._t_allocator);
}

void test__hashtable_is_created__created(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");

    assert_true(_hashtable_is_created(pt_hashtable));

    _hashtable_destroy(pt_hashtable);
}

/*
 * test _hashtable_is_inited
 */
UT_CASE_DEFINATION(_hashtable_is_inited)
void test__hashtable_is_inited__null_hashtable(void** state)
{
    expect_assert_failure(_hashtable_is_inited(NULL));
}

void test__hashtable_is_inited__invalid_typeinfo_style(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");

    _hashtable_init(pt_hashtable, 8, NULL, NULL);

    pt_hashtable->_t_typeinfo._t_style = 999;
    assert_false(_hashtable_is_inited(pt_hashtable));
    pt_hashtable->_t_typeinfo._t_style = _TYPE_C_BUILTIN;

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_is_inited__invalid_typeinfo_type(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _type_t* pt_type = NULL;

    _hashtable_init(pt_hashtable, 0, NULL, NULL);

    pt_type = pt_hashtable->_t_typeinfo._pt_type;
    pt_hashtable->_t_typeinfo._pt_type = NULL;
    assert_false(_hashtable_is_inited(pt_hashtable));
    pt_hashtable->_t_typeinfo._pt_type = pt_type;

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_is_inited__invalid_hash(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    unary_function_t t_hash = NULL;

    _hashtable_init(pt_hashtable, 199, NULL, NULL);

    t_hash = pt_hashtable->_t_hash;
    pt_hashtable->_t_hash = NULL;
    assert_false(_hashtable_is_inited(pt_hashtable));
    pt_hashtable->_t_hash = t_hash;

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_is_inited__invalid_compare(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    binary_function_t t_compare = NULL;

    _hashtable_init(pt_hashtable, 9, NULL, NULL);

    t_compare = pt_hashtable->_t_compare;
    pt_hashtable->_t_compare = NULL;
    assert_false(_hashtable_is_inited(pt_hashtable));
    pt_hashtable->_t_compare = t_compare;

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_is_inited__invalid_bucket(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _byte_t* pby_start = NULL;

    _hashtable_init(pt_hashtable, 3345, NULL, NULL);

    pby_start = pt_hashtable->_t_bucket._pby_start;
    pt_hashtable->_t_bucket._pby_start = NULL;
    assert_false(_hashtable_is_inited(pt_hashtable));
    pt_hashtable->_t_bucket._pby_start = pby_start;

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_is_inited__inited(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");

    _hashtable_init(pt_hashtable, 5, NULL, NULL);

    assert_true(_hashtable_is_inited(pt_hashtable));

    _hashtable_destroy(pt_hashtable);
}

/*
 * test _hashtable_iterator_belong_to_hashtable
 */
UT_CASE_DEFINATION(_hashtable_iterator_belong_to_hashtable)
void test__hashtable_iterator_belong_to_hashtable__null_hashtable(void** state)
{
    _hashtable_iterator_t it_iter;
    expect_assert_failure(_hashtable_iterator_belong_to_hashtable(NULL, it_iter));
}

void test__hashtable_iterator_belong_to_hashtable__non_inited(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;

    _hashtable_init(pt_hashtable, 9, NULL, NULL);
    it_iter = _hashtable_begin(pt_hashtable);
    pt_hashtable->_t_hash = NULL;
    expect_assert_failure(_hashtable_iterator_belong_to_hashtable(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_iterator_belong_to_hashtable__null_bucketpos(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;

    _hashtable_init(pt_hashtable, 9, NULL, NULL);
    it_iter = _hashtable_begin(pt_hashtable);
    it_iter._t_pos._t_hashpos._pc_bucketpos = NULL;
    expect_assert_failure(_hashtable_iterator_belong_to_hashtable(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_iterator_belong_to_hashtable__null_hashtablepos(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;

    _hashtable_init(pt_hashtable, 9, NULL, NULL);
    it_iter = _hashtable_begin(pt_hashtable);
    it_iter._t_pos._t_hashpos._pt_hashtable = NULL;
    expect_assert_failure(_hashtable_iterator_belong_to_hashtable(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_iterator_belong_to_hashtable__begin(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;
    int n_elem = 9;

    _hashtable_init(pt_hashtable, 9, NULL, NULL);
    _hashtable_insert_equal(pt_hashtable, &n_elem);
    it_iter = _hashtable_begin(pt_hashtable);
    assert_true(_hashtable_iterator_belong_to_hashtable(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_iterator_belong_to_hashtable__end(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;
    int n_elem = 9;

    _hashtable_init(pt_hashtable, 9, NULL, NULL);
    _hashtable_insert_equal(pt_hashtable, &n_elem);
    it_iter = _hashtable_end(pt_hashtable);
    assert_true(_hashtable_iterator_belong_to_hashtable(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_iterator_belong_to_hashtable__middle(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;
    int i = 0;

    _hashtable_init(pt_hashtable, 9, NULL, NULL);
    for(i = 0; i < 10; ++i)
    {
        _hashtable_insert_equal(pt_hashtable, &i);
    }
    it_iter = _hashtable_iterator_next(_hashtable_begin(pt_hashtable));
    assert_true(_hashtable_iterator_belong_to_hashtable(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_iterator_belong_to_hashtable__invalid_bucketpos(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;
    int i = 0;

    _hashtable_init(pt_hashtable, 9, NULL, NULL);
    for(i = 0; i < 10; ++i)
    {
        _hashtable_insert_equal(pt_hashtable, &i);
    }
    it_iter = _hashtable_iterator_next(_hashtable_begin(pt_hashtable));
    it_iter._t_pos._t_hashpos._pc_bucketpos = (char*)0x44444;
    assert_false(_hashtable_iterator_belong_to_hashtable(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_iterator_belong_to_hashtable__invalid_corepos(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;
    int i = 0;

    _hashtable_init(pt_hashtable, 9, NULL, NULL);
    for(i = 0; i < 10; ++i)
    {
        _hashtable_insert_equal(pt_hashtable, &i);
    }
    it_iter = _hashtable_iterator_next(_hashtable_begin(pt_hashtable));
    it_iter._t_pos._t_hashpos._pc_corepos = (char*)0x04;
    assert_false(_hashtable_iterator_belong_to_hashtable(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

/*
 * test _hashtable_same_hashtable_iterator_type
 */
UT_CASE_DEFINATION(_hashtable_same_hashtable_iterator_type)
void test__hashtable_same_hashtable_iterator_type__null_hashtable(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    expect_assert_failure(_hashtable_same_hashtable_iterator_type(NULL, _hashtable_begin(pt_hashtable)));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_same_hashtable_iterator_type__non_created(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_t htable;

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    expect_assert_failure(_hashtable_same_hashtable_iterator_type(&htable, _hashtable_begin(pt_hashtable)));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_same_hashtable_iterator_type__invalid_iter(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    it_iter = _hashtable_begin(pt_hashtable);
    it_iter._t_pos._t_hashpos._pt_hashtable = NULL;
    expect_assert_failure(_hashtable_same_hashtable_iterator_type(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_same_hashtable_iterator_type__same_container(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    it_iter = _hashtable_begin(pt_hashtable);
    assert_true(_hashtable_same_hashtable_iterator_type(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_same_hashtable_iterator_type__same(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_t* pt_iter = _create_hashtable("int");

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    _hashtable_init(pt_iter, 100, NULL, NULL);
    assert_true(_hashtable_same_hashtable_iterator_type(pt_hashtable, _hashtable_begin(pt_iter)));

    _hashtable_destroy(pt_hashtable);
    _hashtable_destroy(pt_iter);
}

void test__hashtable_same_hashtable_iterator_type__not_same(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_t* pt_iter = _create_hashtable("double");

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    _hashtable_init(pt_iter, 100, NULL, NULL);
    assert_false(_hashtable_same_hashtable_iterator_type(pt_hashtable, _hashtable_begin(pt_iter)));

    _hashtable_destroy(pt_hashtable);
    _hashtable_destroy(pt_iter);
}

/*
 * test _hashtable_same_hashtable_iterator_type_ex
 */
UT_CASE_DEFINATION(_hashtable_same_hashtable_iterator_type_ex)
void test__hashtable_same_hashtable_iterator_type_ex__null_hashtable(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    expect_assert_failure(_hashtable_same_hashtable_iterator_type_ex(NULL, _hashtable_begin(pt_hashtable)));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_same_hashtable_iterator_type_ex__non_created(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_t htable;

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    expect_assert_failure(_hashtable_same_hashtable_iterator_type_ex(&htable, _hashtable_begin(pt_hashtable)));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_same_hashtable_iterator_type_ex__invalid_iter(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    it_iter = _hashtable_begin(pt_hashtable);
    it_iter._t_pos._t_hashpos._pt_hashtable = NULL;
    expect_assert_failure(_hashtable_same_hashtable_iterator_type_ex(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_same_hashtable_iterator_type_ex__same_container(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_iterator_t it_iter;

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    it_iter = _hashtable_begin(pt_hashtable);
    assert_true(_hashtable_same_hashtable_iterator_type_ex(pt_hashtable, it_iter));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_same_hashtable_iterator_type_ex__same(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_t* pt_iter = _create_hashtable("int");

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    _hashtable_init(pt_iter, 0, NULL, NULL);
    assert_true(_hashtable_same_hashtable_iterator_type_ex(pt_hashtable, _hashtable_begin(pt_iter)));

    _hashtable_destroy(pt_hashtable);
    _hashtable_destroy(pt_iter);
}

void test__hashtable_same_hashtable_iterator_type_ex__not_same_type(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_t* pt_iter = _create_hashtable("double");

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    _hashtable_init(pt_iter, 0, NULL, NULL);
    assert_false(_hashtable_same_hashtable_iterator_type_ex(pt_hashtable, _hashtable_begin(pt_iter)));

    _hashtable_destroy(pt_hashtable);
    _hashtable_destroy(pt_iter);
}

static void _test__hashtable_same_hashtable_iterator_type_ex__not_same_hash(const void* cpv_first, void* pv_output)
{
    pv_output = (void*)cpv_first;
}
void test__hashtable_same_hashtable_iterator_type_ex__not_same_hash(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_t* pt_iter = _create_hashtable("double");

    _hashtable_init(pt_hashtable, 0, NULL, NULL);
    _hashtable_init(pt_iter, 0, _test__hashtable_same_hashtable_iterator_type_ex__not_same_hash, NULL);
    assert_false(_hashtable_same_hashtable_iterator_type_ex(pt_hashtable, _hashtable_begin(pt_iter)));

    _hashtable_destroy(pt_hashtable);
    _hashtable_destroy(pt_iter);
}

static void _test__hashtable_same_hashtable_iterator_type_ex__not_same_compare(const void* cpv_first, const void* cpv_second, void* pv_output)
{
    *(int*)pv_output = *(int*)cpv_first + *(int*)cpv_second;
}
void test__hashtable_same_hashtable_iterator_type_ex__not_same_compare(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");
    _hashtable_t* pt_iter = _create_hashtable("double");

    _hashtable_init(pt_hashtable, 0, NULL, _test__hashtable_same_hashtable_iterator_type_ex__not_same_compare);
    _hashtable_init(pt_iter, 0, NULL, NULL);
    assert_false(_hashtable_same_hashtable_iterator_type_ex(pt_hashtable, _hashtable_begin(pt_iter)));

    _hashtable_destroy(pt_hashtable);
    _hashtable_destroy(pt_iter);
}

/*
 * test _hashtable_same_type
 */
UT_CASE_DEFINATION(_hashtable_same_type)
void test__hashtable_same_type__null_first(void** state)
{
    _hashtable_t htable;

    expect_assert_failure(_hashtable_same_type(NULL, &htable));
}

void test__hashtable_same_type__null_second(void** state)
{
    _hashtable_t htable;

    expect_assert_failure(_hashtable_same_type(&htable, NULL));
}

void test__hashtable_same_type__non_created_first(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("int");

    pt_first->_t_typeinfo._t_style = 9999;
    expect_assert_failure(_hashtable_same_type(pt_first, pt_second));
    pt_first->_t_typeinfo._t_style = _TYPE_C_BUILTIN;

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

void test__hashtable_same_type__non_created_second(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("int");

    pt_second->_t_typeinfo._t_style = 22222;
    expect_assert_failure(_hashtable_same_type(pt_first, pt_second));
    pt_second->_t_typeinfo._t_style = _TYPE_C_BUILTIN;

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

void test__hashtable_same_type__same_type(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("int");

    assert_true(_hashtable_same_type(pt_first, pt_second));

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

void test__hashtable_same_type__same_hashtable(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");

    assert_true(_hashtable_same_type(pt_hashtable, pt_hashtable));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_same_type__not_same_type(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("double");

    assert_false(_hashtable_same_type(pt_first, pt_second));

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

static void _test__hashtable_same_type__not_same_hash(const void* cpv_input, void* pv_output)
{
    pv_output = (void*)cpv_input;
}
void test__hashtable_same_type__not_same_hash(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("int");

    _hashtable_init(pt_first, 0, NULL, NULL);
    _hashtable_init(pt_second, 0, _test__hashtable_same_type__not_same_hash, NULL);
    assert_true(_hashtable_same_type(pt_first, pt_second));

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

static void _test__hashtable_same_type__not_same_compare(const void* cpv_first, const void* cpv_second, void* pv_output)
{
    *(int*)pv_output = *(int*)cpv_first + *(int*)cpv_second;
}
void test__hashtable_same_type__not_same_compare(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("int");

    _hashtable_init(pt_first, 0, NULL, _test__hashtable_same_type__not_same_compare);
    _hashtable_init(pt_second, 0, NULL, NULL);
    assert_true(_hashtable_same_type(pt_first, pt_second));

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

/*
 * test _hashtable_same_type_ex
 */
UT_CASE_DEFINATION(_hashtable_same_type_ex)
void test__hashtable_same_type_ex__null_first(void** state)
{
    _hashtable_t htable;

    expect_assert_failure(_hashtable_same_type_ex(NULL, &htable));
}

void test__hashtable_same_type_ex__null_second(void** state)
{
    _hashtable_t htable;

    expect_assert_failure(_hashtable_same_type_ex(&htable, NULL));
}

void test__hashtable_same_type_ex__non_created_first(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("int");

    pt_first->_t_typeinfo._t_style = 9999;
    expect_assert_failure(_hashtable_same_type_ex(pt_first, pt_second));
    pt_first->_t_typeinfo._t_style = _TYPE_C_BUILTIN;

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

void test__hashtable_same_type_ex__non_created_second(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("int");

    pt_second->_t_typeinfo._t_style = 22222;
    expect_assert_failure(_hashtable_same_type_ex(pt_first, pt_second));
    pt_second->_t_typeinfo._t_style = _TYPE_C_BUILTIN;

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

void test__hashtable_same_type_ex__same_type(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("int");

    assert_true(_hashtable_same_type_ex(pt_first, pt_second));

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

void test__hashtable_same_type_ex__same_hashtable(void** state)
{
    _hashtable_t* pt_hashtable = _create_hashtable("int");

    assert_true(_hashtable_same_type_ex(pt_hashtable, pt_hashtable));

    _hashtable_destroy(pt_hashtable);
}

void test__hashtable_same_type_ex__not_same_type(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("double");

    assert_false(_hashtable_same_type_ex(pt_first, pt_second));

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

static void _test__hashtable_same_type_ex__not_same_hash(const void* cpv_input, void* pv_output)
{
    pv_output = (void*)cpv_input;
}
void test__hashtable_same_type_ex__not_same_hash(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("int");

    _hashtable_init(pt_first, 0, NULL, NULL);
    _hashtable_init(pt_second, 0, _test__hashtable_same_type_ex__not_same_hash, NULL);
    assert_false(_hashtable_same_type_ex(pt_first, pt_second));

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

static void _test__hashtable_same_type_ex__not_same_compare(const void* cpv_first, const void* cpv_second, void* pv_output)
{
    *(int*)pv_output = *(int*)cpv_first + *(int*)cpv_second;
}
void test__hashtable_same_type_ex__not_same_compare(void** state)
{
    _hashtable_t* pt_first = _create_hashtable("int");
    _hashtable_t* pt_second = _create_hashtable("int");

    _hashtable_init(pt_first, 0, NULL, _test__hashtable_same_type_ex__not_same_compare);
    _hashtable_init(pt_second, 0, NULL, NULL);
    assert_false(_hashtable_same_type_ex(pt_first, pt_second));

    _hashtable_destroy(pt_first);
    _hashtable_destroy(pt_second);
}

/*
 * test _hashtable_get_prime
 */
UT_CASE_DEFINATION(_hashtable_get_prime)
void test__hashtable_get_prime__0(void** state)
{
    assert_true(_hashtable_get_prime(0) == 53);
}

void test__hashtable_get_prime__53(void** state)
{
    assert_true(_hashtable_get_prime(53) == 53);
}

void test__hashtable_get_prime__8000000(void** state)
{
    assert_true(_hashtable_get_prime(8000000) == 12582917);
}

void test__hashtable_get_prime__201326611(void** state)
{
    assert_true(_hashtable_get_prime(201326611) == 201326611);
}

void test__hashtable_get_prime__4294967290(void** state)
{
    assert_true(_hashtable_get_prime(4294967290ul) == 4294967291ul);
}

void test__hashtable_get_prime__4294967291(void** state)
{
    assert_true(_hashtable_get_prime(4294967291ul) == 4294967291ul);
}

void test__hashtable_get_prime__4294967295(void** state)
{
    assert_true(_hashtable_get_prime(4294967295ul) == 4294967295ul);
}

