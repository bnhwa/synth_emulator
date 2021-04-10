//=========================================
// wave stuff
//=========================================
#define cnt_notes(x) (sizeof(x) / sizeof(x[0]))
//=========================================
//Notes
//=========================================

#define C 0
#define CS 1
#define D 2
#define DS 3
#define E 4
#define F 5
#define  FS 6
#define  G 7
#define  GS 8
#define  A 9
#define  AS 10
#define  B 11
#define R 12 //rest
byte C_N[][2] = {{C,0}};
byte CS_N[][2] = {{CS,0}};
byte D_N[][2] = {{D,0}};
byte DS_N[][2] = {{DS,0}};
byte E_N[][2] = {{E,0}};
byte F_N[][2] = {{F,0}};
byte  FS_N[][2]= {{FS,0}};
byte  G_N[][2] = {{G,0}};
byte  GS_N[][2] = {{GS,0}};
byte  A_N[][2] = {{A,0}};
byte  AS_N[][2] = {{AS,0}};
byte  B_N[][2] = {{B,0}};
byte R_N[][2]  = {{0,0}}; //rest
byte C_T_[][2] ={{C,0}, {E,0}, {G,0}, {C,1}};//CEG C+ {freq,offset}, freq offse
byte CS_T_[][2]  = {{CS,0}, {F,0}, {GS,0}, {CS,1}};
byte D_T_[][2]  = {{D,0}, {FS,0}, {A,0}, {D,1}};

byte DS_T_[][2]  = {{DS,0}, {F,0}, {AS,0}, {DS,1}};
byte E_T_[][2]  = {{E,0}, {GS,0}, {B,0}, {E,1}};
byte F_T_[][2]  = {{F,0}, {A,0}, {C,1}, {F,1}};
byte FS_T_[][2]  = {{FS,0}, {C,0}, {DS,1}, {FS,1}};
byte G_T_[][2]  = {{G,0}, {B,0}, {D,1}, {G,1}};
byte GS_T_[][2]  = {{GS,0}, {C,1}, {DS,1}, {GS,1}};
byte A_T_[][2]  = {{A,0}, {CS,0}, {E,1}, {A,1}};
byte AS_T_[][2]  = {{AS,0}, {D,0}, {F,1}, {AS,1}};
byte B_T_[][2]  = {{B,0}, {DS,0}, {FS,1}, {B,1}};


byte A_DISS_[][2]  = {{A,0}, {CS,0}, {E,1}, {GS,1}};
byte CS_FOUR_[][2]  = {{CS,0}, {E,0}, {A,0}, {CS,1}};
#define C_T 13 
#define CS_T 14
#define D_T 15

#define DS_T 16
#define E_T 17
#define F_T 18
#define FS_T 19
#define G_T 20
#define GS_T 21
#define A_T 22
#define AS_T 23
#define B_T 24

#define A_DISS 25
#define CS_FOUR 26
//get num notes per each,
#define cnt_notes(x) (sizeof(x) / sizeof(x[0]))
struct NOTES
{
  byte (*notes)[2];
  uint16_t note_count;
};
NOTES noteTable[] = {
  { C_N , cnt_notes(C_N)},
  { CS_N , cnt_notes(CS_N)},
  { D_N , cnt_notes(D_N)},
  { DS_N , cnt_notes(DS_N)},
  { E_N , cnt_notes(E_N)},
  { F_N , cnt_notes(F_N)},
  {  FS_N, cnt_notes(FS_N)},
  {  G_N , cnt_notes(G_N)},
  {  GS_N , cnt_notes(GS_N)},
  {  A_N , cnt_notes(A_N)},
  {  AS_N , cnt_notes(AS_N)},
  {  B_N , cnt_notes(B_N)},
  { R_N  , cnt_notes(C_N)},
  { C_T_, cnt_notes(C_T_)},
  { CS_T_, cnt_notes(CS_T_)},
  { D_T_, cnt_notes(D_T_)},
  { DS_T_, cnt_notes(DS_T_)},
  { E_T_, cnt_notes(E_T_)},
  { F_T_, cnt_notes(F_T_)},
  { FS_T_, cnt_notes(FS_T_)},
  { G_T_, cnt_notes(G_T_)},
  { GS_T_, cnt_notes(GS_T_)},
  { A_T_, cnt_notes(A_T_)},
  { AS_T_, cnt_notes(AS_T_)},
  { B_T_, cnt_notes(B_T_)},
  { A_DISS_, cnt_notes(A_DISS_)},
  { CS_FOUR_, cnt_notes(CS_FOUR_)}

};