/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define SnP_StaticInitialize            JOIN(SnP, _StaticInitialize)
#define SnP_Initialize                  JOIN(SnP, _Initialize)
#define SnP_AddBytes                    JOIN(SnP, _AddBytes)
#define SnP_AddByte                     JOIN(SnP, _AddByte)
#define SnP_OverwriteBytes              JOIN(SnP, _OverwriteBytes)
#define SnP_ExtractBytes                JOIN(SnP, _ExtractBytes)
#define SnP_ExtractAndAddBytes          JOIN(SnP, _ExtractAndAddBytes)

#define Cyclist_Instance                JOIN(prefix, _Instance)
#define Cyclist_Initialize              JOIN(prefix, _Initialize)
#define Cyclist_Absorb                  JOIN(prefix, _Absorb)
#define Cyclist_Encrypt                 JOIN(prefix, _Encrypt)
#define Cyclist_Decrypt                 JOIN(prefix, _Decrypt)
#define Cyclist_Squeeze                 JOIN(prefix, _Squeeze)
#define Cyclist_SqueezeKey              JOIN(prefix, _SqueezeKey)
#define Cyclist_Ratchet                 JOIN(prefix, _Ratchet)

#define Cyclist_AbsorbAny               JOIN(prefix, _AbsorbAny)
#define Cyclist_AbsorbKey               JOIN(prefix, _AbsorbKey)
#define Cyclist_SqueezeAny              JOIN(prefix, _SqueezeAny)
#define Cyclist_Down                    JOIN(prefix, _Down)
#define Cyclist_Up                      JOIN(prefix, _Up)
#define Cyclist_Crypt                   JOIN(prefix, _Crypt)

#define Cyclist_f_bPrime                JOIN(prefix, _f_bPrime)
#define Cyclist_Rhash                   JOIN(prefix, _Rhash)
#define Cyclist_Rkin                    JOIN(prefix, _Rkin)
#define Cyclist_Rkout                   JOIN(prefix, _Rkout)
#define Cyclist_lRatchet                JOIN(prefix, _lRatchet)

/* ------- Cyclist internal interfaces ------- */

static void Cyclist_Down(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *Xi1, uint8_t *Xi0, unsigned int XiLen, uint8_t Cd)
{
    SnP_AddBytes(instance0->state, Xi0, 0, XiLen);
    SnP_AddByte(instance0->state, 0x01, XiLen);
    SnP_AddByte(instance0->state, (instance0->mode == Cyclist_ModeHash) ? (Cd & 0x01) : Cd, Cyclist_f_bPrime - 1);
    instance0->phase = Cyclist_PhaseDown;

    SnP_AddBytes(instance1->state, Xi1, 0, XiLen);
    SnP_AddByte(instance1->state, 0x00, XiLen);  //0x01 just add once
    SnP_AddByte(instance1->state, 0x00, Cyclist_f_bPrime - 1);  //Color just add once
    instance1->phase = Cyclist_PhaseDown;

}

static void Cyclist_Up(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *Yi1, uint8_t *Yi0, unsigned int YiLen, uint8_t Cu)
{
    #if defined(OUTPUT)
    uint8_t   s0[Cyclist_f_bPrime];
    uint8_t   s1[Cyclist_f_bPrime];
    #endif

    if (instance0->mode != Cyclist_ModeHash) {
        SnP_AddByte(instance0->state, Cu, Cyclist_f_bPrime - 1);
    }
    if (instance1->mode != Cyclist_ModeHash) {
        SnP_AddByte(instance1->state, 0x00, Cyclist_f_bPrime - 1);
    }
    #if defined(OUTPUT)
    if (instance0->file != NULL) {
        SnP_ExtractBytes( instance0->stateShadow, s0, 0, Cyclist_f_bPrime );
        SnP_ExtractAndAddBytes( instance0->state, s0, s0, 0, Cyclist_f_bPrime );
    }
    if (instance1->file != NULL) {
        SnP_ExtractBytes( instance1->stateShadow, s1, 0, Cyclist_f_bPrime );
        SnP_ExtractAndAddBytes( instance1->state, s1, s1, 0, Cyclist_f_bPrime );
    }
    #endif
    SnP_Permute( instance1->state, instance0->state );
    #if defined(OUTPUT)
    if (instance0->file != NULL) {
        memcpy( instance0->stateShadow, instance0->state, sizeof(instance0->state) );
        fprintf( instance0->file, "Data XORed" );
        displayByteString( instance0->file, "", s0, Cyclist_f_bPrime );
        SnP_ExtractBytes( instance0->stateShadow, s0, 0, Cyclist_f_bPrime );
        fprintf( instance0->file, "After f() ");
        displayByteString( instance0->file, "", s0, Cyclist_f_bPrime );
    }
    if (instance1->file != NULL) {
        memcpy( instance1->stateShadow, instance1->state, sizeof(instance1->state) );
        fprintf( instance1->file, "Data XORed" );
        displayByteString( instance1->file, "", s1, Cyclist_f_bPrime );
        SnP_ExtractBytes( instance1->stateShadow, s1, 0, Cyclist_f_bPrime );
        fprintf( instance1->file, "After f() ");
        displayByteString( instance1->file, "", s1, Cyclist_f_bPrime );
    }
    #endif
    instance0->phase = Cyclist_PhaseUp;
    SnP_ExtractBytes( instance0->state, Yi0, 0, YiLen );

    instance1->phase = Cyclist_PhaseUp;
    SnP_ExtractBytes( instance1->state, Yi1, 0, YiLen );
}

static void Cyclist_AbsorbAny(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *X1, uint8_t *X0, size_t XLen, unsigned int r, uint8_t Cd)
{
    unsigned int splitLen;

    do {
        if (instance0->phase != Cyclist_PhaseUp) {
            Cyclist_Up(instance1, instance0, NULL, NULL, 0, 0);
        }
        splitLen = MyMin(XLen, r);
        Cyclist_Down(instance1, instance0, X1, X0, splitLen, Cd);
        Cd = 0;
        X0       += splitLen;
        X1       += splitLen;
        XLen    -= splitLen;
    } while ( XLen != 0 );
}

static void Cyclist_AbsorbKey(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *K1, uint8_t *K0, size_t KLen, uint8_t *ID1, uint8_t *ID0, size_t IDLen, uint8_t *counter, size_t counterLen)
{
    uint8_t KID0[Cyclist_Rkin];
    uint8_t KID1[Cyclist_Rkin];
    uint8_t zero[Cyclist_Rkin] = {0};

    assert(instance0->mode == Cyclist_ModeHash);
    assert(instance1->mode == Cyclist_ModeHash);
    assert((KLen + IDLen) <= (Cyclist_Rkin - 1));

    instance0->mode      = Cyclist_ModeKeyed;
    instance0->Rabsorb   = Cyclist_Rkin;
    instance0->Rsqueeze  = Cyclist_Rkout;

    instance1->mode      = Cyclist_ModeKeyed;
    instance1->Rabsorb   = Cyclist_Rkin;
    instance1->Rsqueeze  = Cyclist_Rkout;
    if (KLen != 0) {
        memcpy(KID0, K0, KLen);
        memcpy(KID0 + KLen, ID0, IDLen);
        memcpy(KID1, K1, KLen);
        memcpy(KID1 + KLen, ID1, IDLen); 
        KID0[KLen + IDLen] = (uint8_t)IDLen;
        KID1[KLen + IDLen] = (uint8_t)0x00;

        Cyclist_AbsorbAny(instance1, instance0, KID1, KID0, KLen + IDLen + 1, instance0->Rabsorb, 0x02);
        if (counterLen != 0) {
            Cyclist_AbsorbAny(instance1, instance0, zero, counter, counterLen, 1, 0x00);
        }
    }
}

static void Cyclist_SqueezeAny(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *Y1, uint8_t *Y0, size_t YLen, uint8_t Cu)
{
    unsigned int len;

    len = MyMin(YLen, instance0->Rsqueeze );
    Cyclist_Up(instance1, instance0, Y1, Y0, len, Cu);
    Y0 += len;
    Y1 += len;
    YLen -= len;
    while (YLen != 0) {
        Cyclist_Down(instance1, instance0, NULL, NULL, 0, 0);
        len = MyMin(YLen, instance0->Rsqueeze );
        Cyclist_Up(instance1, instance0, Y1, Y0, len, 0);
        Y0 += len;
        Y1 += len;
        YLen -= len;
    }
}

static void Cyclist_Crypt(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *I1, uint8_t *I0, uint8_t *O1, uint8_t *O0, size_t IOLen, int decrypt)
{
    unsigned int splitLen;
    uint8_t      P0[Cyclist_Rkout];
    uint8_t      P1[Cyclist_Rkout];
    uint8_t      Cu = 0x80;

    do {
        splitLen = MyMin(IOLen, Cyclist_Rkout); /* use Rkout instead of Rsqueeze, this function is only called in keyed mode */
        if (decrypt != 0) {
            Cyclist_Up(instance1, instance0, NULL, NULL, 0, Cu); /* Up without extract */
            Xoodoo_ExtractAndAddBytes(instance0->state, I0, O0, 0, splitLen); /* Extract from Up and Add */
            Xoodoo_ExtractAndAddBytes(instance1->state, I1, O1, 0, splitLen); /* Extract from Up and Add */
            Cyclist_Down(instance1, instance0, O1, O0, splitLen, 0x00);
        }
        else {
            memcpy(P0, I0, splitLen);
            memcpy(P1, I1, splitLen);
            Cyclist_Up(instance1, instance0, NULL, NULL, 0, Cu); /* Up without extract */
            Xoodoo_ExtractAndAddBytes(instance0->state, I0, O0, 0, splitLen); /* Extract from Up and Add */
            Xoodoo_ExtractAndAddBytes(instance1->state, I1, O1, 0, splitLen); /* Extract from Up and Add */
            Cyclist_Down(instance1, instance0, P1, P0, splitLen, 0x00);
        }
        Cu = 0x00;
        I0       += splitLen;
        I1       += splitLen;
        O0       += splitLen;
        O1       += splitLen;
        IOLen   -= splitLen;
    } while ( IOLen != 0 );
}

/* ------- Cyclist interfaces ------- */

void Cyclist_Initialize(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *K1, uint8_t *K0, size_t KLen, uint8_t *ID1, uint8_t *ID0, size_t IDLen, uint8_t *counter, size_t counterLen)
{
    SnP_StaticInitialize();
    SnP_Initialize(instance1->state, instance0->state);
    instance0->phase     = Cyclist_PhaseUp;
    instance0->mode      = Cyclist_ModeHash;
    instance0->Rabsorb   = Cyclist_Rhash;
    instance0->Rsqueeze  = Cyclist_Rhash;

    instance1->phase     = Cyclist_PhaseUp;
    instance1->mode      = Cyclist_ModeHash;
    instance1->Rabsorb   = Cyclist_Rhash;
    instance1->Rsqueeze  = Cyclist_Rhash;

    #ifdef OUTPUT
    instance0->file = 0;
    instance1->file = 0;
    SnP_Initialize( instance1->stateShadow, instance0->stateShadow );
    #endif

    if (KLen != 0) {
        Cyclist_AbsorbKey(instance1, instance0, K1, K0, KLen, ID1, ID0, IDLen, counter, counterLen);
    }
}

void Cyclist_Absorb(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *X1, uint8_t *X0, size_t XLen)
{
    Cyclist_AbsorbAny(instance1, instance0, X1, X0, XLen, instance0->Rabsorb, 0x03);
}

void Cyclist_Encrypt(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *P1, uint8_t *P0, uint8_t *C1, uint8_t *C0, size_t PLen)
{
    assert(instance0->mode == Cyclist_ModeKeyed);
    Cyclist_Crypt(instance1, instance0, P1, P0, C1, C0, PLen, 0);
}

void Cyclist_Decrypt(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *C1, uint8_t *C0, uint8_t *P1, uint8_t *P0, size_t CLen)
{
    assert(instance0->mode == Cyclist_ModeKeyed);
    Cyclist_Crypt(instance1, instance0, C1, C0, P1, P0, CLen, 1);
}

void Cyclist_Squeeze(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *Y1, uint8_t *Y0, size_t YLen)
{
    Cyclist_SqueezeAny(instance1, instance0, Y1, Y0, YLen, 0x40);
}

void Cyclist_SqueezeKey(Cyclist_Instance *instance1, Cyclist_Instance *instance0, uint8_t *K1, uint8_t *K0, size_t KLen)
{
    assert(instance0->mode == Cyclist_ModeKeyed);
    Cyclist_SqueezeAny(instance1, instance0, K1, K0, KLen, 0x20);
}

void Cyclist_Ratchet(Cyclist_Instance *instance1, Cyclist_Instance *instance0)
{
    uint8_t buffer0[Cyclist_lRatchet];
    uint8_t buffer1[Cyclist_lRatchet];

    assert(instance0->mode == Cyclist_ModeKeyed);
    /* Squeeze then absorb is the same as overwriting with zeros */
    Cyclist_SqueezeAny(instance1, instance0, buffer1, buffer0, sizeof(buffer0), 0x10);
    Cyclist_AbsorbAny(instance1, instance0, buffer1, buffer0, sizeof(buffer0), instance0->Rabsorb, 0x00);
}

#undef SnP_StaticInitialize
#undef SnP_Initialize
#undef SnP_AddBytes
#undef SnP_AddByte
#undef SnP_OverwriteBytes
#undef SnP_ExtractBytes
#undef SnP_ExtractAndAddBytes

#undef Cyclist_Instance
#undef Cyclist_Initialize
#undef Cyclist_Absorb
#undef Cyclist_Encrypt
#undef Cyclist_Decrypt
#undef Cyclist_Squeeze
#undef Cyclist_SqueezeKey
#undef Cyclist_Ratchet

#undef Cyclist_AbsorbAny
#undef Cyclist_AbsorbKey
#undef Cyclist_SqueezeAny
#undef Cyclist_Down
#undef Cyclist_Up
#undef Cyclist_Crypt

#undef Cyclist_f_bPrime
#undef Cyclist_Rhash
#undef Cyclist_Rkin
#undef Cyclist_Rkout
#undef Cyclist_lRatchet
