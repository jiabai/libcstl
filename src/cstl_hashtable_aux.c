/*
 *  The implementation of hashtable auxiliary functions.
 *  Copyright (C)  2008,2009,2010,2011  Wangbo
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  Author e-mail: activesys.wb@gmail.com
 *                 activesys@sina.com.cn
 */

/** include section **/
#include <cstl/cstl_def.h>
#include <cstl/cstl_alloc.h>
#include <cstl/cstl_types.h>
#include <cstl/cstl_iterator.h>

#include <cstl/cstl_vector_iterator.h>
#include <cstl/cstl_vector_private.h>
#include <cstl/cstl_vector.h>

#include <cstl/cstl_hashtable_iterator.h>
#include <cstl/cstl_hashtable_private.h>

#include <cstl/cstring.h>

#include "cstl_vector_aux.h"
#include "cstl_hashtable_aux.h"

/** local constant declaration and local macro section **/
#define _HASHTABLE_FIRST_PRIME_BUCKET_COUNT 53
#define _HASHTABLE_LAST_PRIME_BUCKET_COUNT  4294967291ul

/** local data type declaration and local struct, union, enum section **/

/** local function prototype section **/

/** exported global variable definition section **/

/** local global variable definition section **/
static const unsigned long _hashtable_prime_list[_HASHTABLE_PRIME_LIST_COUNT] =
{
    53,         97,           193,         389,       769,
    1543,       3079,         6151,        12289,     24593,
    49157,      98317,        196613,      393241,    786433,
    1572869,    3145739,      6291469,     12582917,  25165843,
    50331653,   100663319,    201326611,   402653189, 805306457,
    1610612741, 3221225473ul, 4294967291ul
};

/** exported function implementation section **/
#ifndef NDEBUG
/**
 * Test hashtable is created by _create_hashtable.
 */
bool_t _hashtable_is_created(const _hashtable_t* cpt_hashtable)
{
    assert(cpt_hashtable != NULL);

    if(cpt_hashtable->_t_typeinfo._t_style != _TYPE_C_BUILTIN &&
       cpt_hashtable->_t_typeinfo._t_style != _TYPE_CSTL_BUILTIN &&
       cpt_hashtable->_t_typeinfo._t_style != _TYPE_USER_DEFINE)
    {
        return false;
    }
    if(cpt_hashtable->_t_typeinfo._pt_type == NULL)
    {
        return false;
    }

    if(!_vector_is_created(&cpt_hashtable->_t_bucket))
    {
        return false;
    }

    if(cpt_hashtable->_t_nodecount != 0 || cpt_hashtable->_t_hash != NULL || cpt_hashtable->_t_compare != NULL)
    {
        return false;
    }

    return _alloc_is_inited(&cpt_hashtable->_t_allocator);
}

/**
 * Test hashtable is initialized by hashtable initialization functions.
 */
bool_t _hashtable_is_inited(const _hashtable_t* cpt_hashtable)
{
    assert(cpt_hashtable != NULL);

    if(cpt_hashtable->_t_typeinfo._t_style != _TYPE_C_BUILTIN &&
       cpt_hashtable->_t_typeinfo._t_style != _TYPE_CSTL_BUILTIN &&
       cpt_hashtable->_t_typeinfo._t_style != _TYPE_USER_DEFINE)
    {
        return false;
    }
    if(cpt_hashtable->_t_typeinfo._pt_type == NULL)
    {
        return false;
    }

    if(!_vector_is_inited(&cpt_hashtable->_t_bucket) ||
       vector_size(&cpt_hashtable->_t_bucket) < _HASHTABLE_FIRST_PRIME_BUCKET_COUNT)
    {
        return false;
    }

    if(cpt_hashtable->_t_hash == NULL || cpt_hashtable->_t_compare == NULL)
    {
        return false;
    }

    return true;
}

/**
 * Test iterator referenced data is within the hashtable.
 */
bool_t _hashtable_iterator_belong_to_hashtable(const _hashtable_t* cpt_hashtable, _hashtable_iterator_t it_iter)
{
    vector_iterator_t it_bucket;

    assert(cpt_hashtable != NULL);
    assert(_hashtable_is_inited(cpt_hashtable));
    assert(_GET_HASHTABLE_BUCKETPOS(it_iter) != NULL);
    assert(_GET_HASHTABLE(it_iter) == cpt_hashtable);

    /* check for the end node */
    it_bucket = vector_end(&cpt_hashtable->_t_bucket);
    if(_GET_VECTOR_COREPOS(it_bucket) == _GET_HASHTABLE_BUCKETPOS(it_iter) &&
       _GET_HASHTABLE_COREPOS(it_iter) == NULL)
    {
        return true;
    }
    else
    {
        _hashnode_t* pt_node = NULL;
        for(it_bucket = vector_begin(&cpt_hashtable->_t_bucket);
            !iterator_equal(it_bucket, vector_end(&cpt_hashtable->_t_bucket));
            it_bucket = iterator_next(it_bucket))
        {
            if(_GET_HASHTABLE_BUCKETPOS(it_iter) == _GET_VECTOR_COREPOS(it_bucket))
            {
                pt_node = *(_hashnode_t**)_GET_VECTOR_COREPOS(it_bucket);
                while(pt_node != NULL)
                {
                    if(pt_node == (_hashnode_t*)_GET_HASHTABLE_COREPOS(it_iter))
                    {
                        return true;
                    }

                    pt_node = pt_node->_pt_next;
                }
            }
        }

        return false;
    }
}

/**
 * Test the type that saved in the hashtable container and referenced by it_iter are same.
 */
bool_t _hashtable_same_hashtable_iterator_type(const _hashtable_t* cpt_hashtable, _hashtable_iterator_t it_iter)
{
    assert(cpt_hashtable != NULL);
    assert(_GET_HASHTABLE(it_iter) != NULL);

    return _hashtable_same_type(cpt_hashtable, _GET_HASHTABLE(it_iter));
}

/**
 * Test the type that saved in the hashtable container and referenced by it_iter are same.
 */
bool_t _hashtable_same_hashtable_iterator_type_ex(const _hashtable_t* cpt_hashtable, _hashtable_iterator_t it_iter)
{
    assert(cpt_hashtable != NULL);
    assert(_GET_HASHTABLE(it_iter) != NULL);

    return _hashtable_same_type_ex(cpt_hashtable, _GET_HASHTABLE(it_iter));
}
#endif /* NDEBUG */

/**
 * Test the type that saved in the hashtable container is same.
 */
bool_t _hashtable_same_type(const _hashtable_t* cpt_first, const _hashtable_t* cpt_second)
{
    assert(cpt_first != NULL);
    assert(cpt_second != NULL);
    assert(_hashtable_is_inited(cpt_first) || _hashtable_is_created(cpt_first));
    assert(_hashtable_is_inited(cpt_second) || _hashtable_is_created(cpt_second));

    if(cpt_first == cpt_second)
    {
        return true;
    }

    return (cpt_first->_t_typeinfo._pt_type == cpt_second->_t_typeinfo._pt_type) &&
           (cpt_first->_t_typeinfo._t_style == cpt_second->_t_typeinfo._t_style) &&
           _type_is_same(_GET_HASHTABLE_TYPE_NAME(cpt_first), _GET_HASHTABLE_TYPE_NAME(cpt_second));
}

/**
 * Test the type and compare function that saved in the hashtable container is same.
 */
bool_t _hashtable_same_type_ex(const _hashtable_t* cpt_first, const _hashtable_t* cpt_second)
{
    assert(cpt_first != NULL);
    assert(cpt_second != NULL);
    assert(_hashtable_is_inited(cpt_first) || _hashtable_is_created(cpt_first));
    assert(_hashtable_is_inited(cpt_second) || _hashtable_is_created(cpt_second));

    if(cpt_first == cpt_second)
    {
        return true;
    }

    return (cpt_first->_t_typeinfo._pt_type == cpt_second->_t_typeinfo._pt_type) &&
           (cpt_first->_t_typeinfo._t_style == cpt_second->_t_typeinfo._t_style) &&
           (cpt_first->_t_hash == cpt_second->_t_hash) &&
           (cpt_first->_t_compare == cpt_second->_t_compare) &&
           _type_is_same(_GET_HASHTABLE_TYPE_NAME(cpt_first), _GET_HASHTABLE_TYPE_NAME(cpt_second));
}

/**
 * Get the next prime base the ul_basenum.
 */
unsigned long _hashtable_get_prime(unsigned long ul_basenum)
{
    int i;
    for(i = 0; i < _HASHTABLE_PRIME_LIST_COUNT; ++i)
    {
        if(_hashtable_prime_list[i] >= ul_basenum)
        {
            return _hashtable_prime_list[i];
        }
    }

    return ul_basenum;
}

void _hashtable_default_hash(const void* cpv_input, void* pv_output)
{
    size_t t_sum = 0;
    char*  pc_value = NULL;
    size_t t_index = 0;

    assert(cpv_input != NULL && pv_output != NULL);

    pc_value = (char*)cpv_input;
    for(t_index = 0; t_index < *(size_t*)pv_output; ++t_index)
    {
        t_sum += (size_t)pc_value[t_index];
    }

    *(size_t*)pv_output = t_sum;
}

void _hashnode_init(const void* cpv_input, void* pv_output)
{
    assert(cpv_input != NULL && pv_output != NULL);

    *(_hashnode_t**)cpv_input = NULL;
    *(bool_t*)pv_output = true;
}

void _hashnode_copy(const void* cpv_first, const void* cpv_second, void* pv_output)
{
    assert(cpv_first != NULL && cpv_second != NULL && pv_output != NULL);

    *(_hashnode_t**)cpv_first = *(_hashnode_t**)cpv_second;
    *(bool_t*)pv_output = true;
}

void _hashnode_less(const void* cpv_first, const void* cpv_second, void* pv_output)
{
    assert(cpv_first != NULL && cpv_second != NULL && pv_output != NULL);

    *(bool_t*)pv_output = *(_hashnode_t**)cpv_first < *(_hashnode_t**)cpv_second ? true : false;
}

void _hashnode_destroy(const void* cpv_input, void* pv_output)
{
    assert(cpv_input != NULL && pv_output != NULL);

    *(_hashnode_t**)cpv_input = NULL;
    *(bool_t*)pv_output = true;
}

void _hashtable_init_elem_auxiliary(_hashtable_t* pt_hashtable, _hashnode_t* pt_node)
{
    assert(pt_hashtable != NULL && pt_node != NULL);

    /* initialize new elements */
    if(_GET_HASHTABLE_TYPE_STYLE(pt_hashtable) == _TYPE_CSTL_BUILTIN)
    {
        /* get element type name */
        char s_elemtypename[_TYPE_NAME_SIZE + 1];
        _type_get_elem_typename(_GET_HASHTABLE_TYPE_NAME(pt_hashtable), s_elemtypename);

        _GET_HASHTABLE_TYPE_INIT_FUNCTION(pt_hashtable)(pt_node->_pc_data, s_elemtypename);
    }
    else
    {
        bool_t t_result = _GET_HASHTABLE_TYPE_SIZE(pt_hashtable);
        _GET_HASHTABLE_TYPE_INIT_FUNCTION(pt_hashtable)(pt_node->_pc_data, &t_result);
        assert(t_result);
    }
}

void _hashtable_hash_auxiliary(const _hashtable_t* cpt_hashtable, const void* cpv_input, void* pv_output)
{
    assert(cpt_hashtable != NULL && cpv_input != NULL && pv_output != NULL);

    if(strncmp(_GET_HASHTABLE_TYPE_NAME(cpt_hashtable), _C_STRING_TYPE, _TYPE_NAME_SIZE) == 0)
    {
        *(size_t*)pv_output = strlen(string_c_str((string_t*)cpv_input));
        cpt_hashtable->_t_hash(string_c_str((string_t*)cpv_input), pv_output);
    }
    else
    {
        cpt_hashtable->_t_hash(cpv_input, pv_output);
    } 
}

void _hashtable_elem_compare_auxiliary(const _hashtable_t* cpt_hashtable,
    const void* cpv_first, const void* cpv_second, void* pv_output)
{
    assert(cpt_hashtable != NULL && cpv_first != NULL && cpv_second != NULL && pv_output != NULL);

    if(strncmp(_GET_HASHTABLE_TYPE_NAME(cpt_hashtable), _C_STRING_TYPE, _TYPE_NAME_SIZE) == 0 &&
       cpt_hashtable->_t_compare != _GET_HASHTABLE_TYPE_LESS_FUNCTION(cpt_hashtable))
    {
        cpt_hashtable->_t_compare(string_c_str((string_t*)cpv_first), string_c_str((string_t*)cpv_second), pv_output);
    }
    else
    {
        cpt_hashtable->_t_compare(cpv_first, cpv_second, pv_output);
    }
}

/** local function implementation section **/

/** eof **/

