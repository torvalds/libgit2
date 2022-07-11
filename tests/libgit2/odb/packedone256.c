#include "clar_libgit2.h"
#include "git2/odb_backend.h"

#include "pack_data_one256.h"
#include "pack.h"

static git_odb *_odb;

void test_odb_packedone256__initialize(void)
{
	git_odb_backend *backend = NULL;

	cl_git_pass(git_odb__new(&_odb, NULL));
	cl_git_pass(git_odb_backend_one_pack(&backend, cl_fixture("testrepo.git/objects/pack/pack-e2f07f30db7e480ea84a0e64ee791b9b270067124b2609019b74f33f256f33fa.idx")));
	cl_git_pass(git_odb_add_backend(_odb, backend, 1));
}

void test_odb_packedone256__cleanup(void)
{
	git_odb_free(_odb);
	_odb = NULL;
}

void test_odb_packedone256__mass_read(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(packed_objects_one256); ++i) {
		git_oid id;
		git_odb_object *obj;

		cl_git_pass(git_oid__fromstr(&id, packed_objects_one256[i], GIT_OID_SHA1));
		cl_assert(git_odb_exists(_odb, &id) == 1);
		cl_git_pass(git_odb_read(&obj, _odb, &id));

		git_odb_object_free(obj);
	}
}

void test_odb_packedone256__read_header_0(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(packed_objects_one256); ++i) {
		git_oid id;
		git_odb_object *obj;
		size_t len;
		git_object_t type;

		cl_git_pass(git_oid__fromstr(&id, packed_objects_one256[i], GIT_OID_SHA1));

		cl_git_pass(git_odb_read(&obj, _odb, &id));
		cl_git_pass(git_odb_read_header(&len, &type, _odb, &id));

		cl_assert(obj->cached.size == len);
		cl_assert(obj->cached.type == type);

		git_odb_object_free(obj);
	}
}
