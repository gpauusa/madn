#include <stdio.h>
#include <assert.h>
#include "madn_main_struct.h"

int main()
{
    printf("Testing madn_main_struct...\n");
    int i;

    //Testing sizeof and DIM values
    assert(PIECES_DIM == sizeof(PIECES));
    assert(DATA_DIM == sizeof(DATA_PAYLOAD));
    assert(MADN_BLOOM_DIM == sizeof(MADN_BLOOM));

    //Testing create APIs
    MADN_INSTANCE* madn_instance = create_madn_instance();
    assert(madn_instance != NULL);
    MADN_BLOOM* madn_bloom = create_madn_bloom();
    assert(madn_bloom != NULL);
    CID* cid = create_cid();
    assert(cid != NULL);
    SID* sid = create_sid();
    assert(sid != NULL);
    MADN_DATAID* dataid = create_dataid();
    assert(dataid != NULL);
    PIECES* pieces = create_pieces();
    assert(pieces != NULL);

    //Creating another set for testing cmp APIs
    MADN_BLOOM* madn_bloom1 = create_madn_bloom();
    assert(madn_bloom1 != NULL);
    CID* cid1 = create_cid();
    assert(cid1 != NULL);
    SID* sid1 = create_sid();
    assert(sid1 != NULL);
    MADN_DATAID* dataid1 = create_dataid();
    assert(dataid1 != NULL);
    PIECES* pieces1 = create_pieces();
    assert(pieces1 != NULL);

    PIECES* pieces2 = create_pieces();
    assert(pieces2 != NULL);

    //Testing CID compare and copy
    *cid = 0x0f0f0f0f;
    *cid1 = 0xdeadbeef;
    assert(cmp_cid(cid, cid1) != 0);
    assert(cmp_cid(cid1, cid) != 0);
    *cid = 0xdeadbeef;
    assert(cmp_cid(cid, cid1) == 0);
    assert(cmp_cid(cid1, cid) == 0);
    *cid1 = 0xbadbadc0;
    copy_cid(cid, cid1);
    assert(*cid == 0xbadbadc0);

    //Testing SID compare and copy
    *sid = 0x0f0f0f0f;
    *sid1 = 0xdeadbeef;
    assert(cmp_sid(sid, sid1) != 0);
    assert(cmp_sid(sid1, sid) != 0);
    *sid = 0xdeadbeef;
    assert(cmp_sid(sid, sid1) == 0);
    assert(cmp_sid(sid1, sid) == 0);
    *sid1 = 0xbadbadc0;
    copy_sid(sid, sid1);
    assert(*sid == 0xbadbadc0);

    //Testing MADN_BLOOM compare and copy
    MADN_PTR bloomptr = (MADN_PTR) madn_bloom;
    MADN_PTR bloomptr1 = (MADN_PTR) madn_bloom1;
    for (i = 0; i < MADN_BLOOM_DIM; i++)
    {
        *bloomptr = i;
        *bloomptr1 = 0;
        bloomptr++;
        bloomptr1++;
    }
    assert(cmp_bloom(madn_bloom, madn_bloom1) != 0);
    copy_bloom(madn_bloom1, madn_bloom);
    assert(cmp_bloom(madn_bloom, madn_bloom1) == 0);
    bloomptr = (MADN_PTR) madn_bloom;
    bloomptr1 = (MADN_PTR) madn_bloom1;
    for (i = 0; i < MADN_BLOOM_DIM; i++)
    {
        assert(*bloomptr == i);
        assert(*bloomptr1 == i);
        bloomptr++;
        bloomptr1++;
    }
 
    //Testing PIECES compare and copy
    MADN_PTR piecesptr = (MADN_PTR) pieces;
    MADN_PTR piecesptr1 = (MADN_PTR) pieces1;
    for (i = 0; i < PIECES_DIM; i++)
    {
        *piecesptr = i;
        *piecesptr1 = 0;
        piecesptr++;
        piecesptr1++;
    }
    assert(cmp_pieces(pieces, pieces1) != 0);
    copy_pieces(pieces1, pieces);
    assert(cmp_pieces(pieces, pieces1) == 0);
    piecesptr = (MADN_PTR) pieces;
    piecesptr1 = (MADN_PTR) pieces1;
    for (i = 0; i < PIECES_DIM; i++)
    {
        assert(*piecesptr == i);
        assert(*piecesptr1 == i);
        piecesptr++;
        piecesptr1++;
    }

    //Testing PIECES dif and complementary
    complementary_pieces(pieces, pieces1); //pieces1 = ~pieces
    dif_pieces(pieces, pieces1, pieces2); //pieces2 = pieces & ~(~pieces)
    assert(cmp_pieces(pieces, pieces2) == 0);

    //checking collapse function
    uint32_t arr[8]; //32 bytes
    uint32_t xor_res = 0;
    for (i = 0; i < 8; i++)
    {
        arr[i] = 45 + i;
        xor_res ^= arr[i];
        //printf("XOR1: %u %u\n", arr[i], xor_res);
    }

    copy_pieces(pieces, (PIECES*) arr);
    //printf("DBG: XOR results: %d %d\n", xor_res, collapse_pieces(pieces));
    assert(collapse_pieces(pieces) == xor_res);

    //Testing MADN_DATAID
    dataid->chunk = 0x0f0f0f0f;
    dataid->stripe = 0xa5a5a5a5;
    dataid1->chunk = 0xdeadbeef;
    dataid1->stripe = 0xcafebabe;
    assert(cmp_dataid(dataid, dataid1) != 0);
    assert(cmp_dataid(dataid1, dataid) != 0);
    dataid->chunk = 0xdeadbeef;
    dataid->stripe = 0xcafebabe;
    assert(cmp_dataid(dataid, dataid1) == 0);
    assert(cmp_dataid(dataid1, dataid) == 0);
    dataid1->chunk = 0xbadbadc0;
    dataid1->stripe = 0x0cbadbad;
    copy_dataid(dataid, dataid1);
    assert(dataid->chunk == 0xbadbadc0);
    assert(dataid->stripe == 0x0cbadbad);


    //Testing free APIs
    free_madn_bloom(&madn_bloom);
    free_cid(&cid);
    free_sid(&sid);
    free_dataid(&dataid);
    free_pieces(&pieces);
    free_madn_bloom(&madn_bloom1);
    free_cid(&cid1);
    free_sid(&sid1);
    free_dataid(&dataid1);
    free_pieces(&pieces1);
    free_pieces(&pieces2);

    assert(madn_bloom == NULL);
    assert(cid == NULL);
    assert(sid == NULL);
    assert(dataid == NULL);
    assert(pieces == NULL);
    assert(madn_bloom1 == NULL);
    assert(cid1 == NULL);
    assert(sid1 == NULL);
    assert(dataid1 == NULL);
    assert(pieces1 == NULL);
    assert(pieces2 == NULL);

    printf("Success!\n");
    return 0;
}
