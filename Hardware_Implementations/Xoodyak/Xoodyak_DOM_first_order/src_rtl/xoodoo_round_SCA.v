/*===========================================================================*\
           Filename : xoodoo_round_SCA.v
             Author : Shuying Yin <yinshuying@mail.tsinghua.edu.cn>
        Description : One round of Xoodoo
          Called by : xoodoo_n_rounds_SCA
   Revision History : 2022-04-18, Revision 0.1.0, Shuying Yin
            License : GNU General Public License v3.0 (GPL-3.0)
                      For more information please see:
                      https://spdx.org/licenses/GPL-3.0.html
\*===========================================================================*/

module xoodoo_round_SCA
(
    input           clk        ,
    input           rst        ,
    input  [ 383:0] in_0       ,
    input  [ 383:0] in_1       ,
    input  [ 383:0] rs         ,
    input  [  31:0] rconst     ,
    output [ 383:0] out_0      ,
    output [ 383:0] out_1
);

    wire [  31:0] A0   [0:3 ];
    wire [  31:0] A1   [0:3 ];
    wire [  31:0] B0   [0:3 ];
    wire [  31:0] B1   [0:3 ];

    wire [  31:0] I1_0 [0:3 ];
    wire [  31:0] I1_1 [0:3 ];

    wire [  31:0] C0   [0:11];
    wire [  31:0] C1   [0:11];
    reg  [  31:0] D0   [0:11];
    reg  [  31:0] D1   [0:11];

    wire [  31:0] rs_s [0:11];

    wire [  31:0] Si_0 [0:11];
    wire [  31:0] Si_1 [0:11];

    wire [  31:0] So_0 [0:11];
    wire [  31:0] So_1 [0:11];

    genvar i;

    integer j;

    generate
    for(i=0;i<12;i=i+1) begin:L0
        assign rs_s[i] = rs[32*i+:32];
        assign Si_0[i] = in_0[32*i+:32];
        assign Si_1[i] = in_1[32*i+:32];
        assign out_0[32*i+:32] = So_0[i];
        assign out_1[32*i+:32] = So_1[i];
    end
    endgenerate

    assign A0[0] = Si_0[00] ^ Si_0[04] ^ Si_0[08];
    assign A0[1] = Si_0[01] ^ Si_0[05] ^ Si_0[09];
    assign A0[2] = Si_0[02] ^ Si_0[06] ^ Si_0[10];
    assign A0[3] = Si_0[03] ^ Si_0[07] ^ Si_0[11];

    assign A1[0] = Si_1[00] ^ Si_1[04] ^ Si_1[08];
    assign A1[1] = Si_1[01] ^ Si_1[05] ^ Si_1[09];
    assign A1[2] = Si_1[02] ^ Si_1[06] ^ Si_1[10];
    assign A1[3] = Si_1[03] ^ Si_1[07] ^ Si_1[11];

    assign B0[0] = {A0[3][26:0],A0[3][31:27]} ^ {A0[3][17:0],A0[3][31:18]};
    assign B0[1] = {A0[0][26:0],A0[0][31:27]} ^ {A0[0][17:0],A0[0][31:18]};
    assign B0[2] = {A0[1][26:0],A0[1][31:27]} ^ {A0[1][17:0],A0[1][31:18]};
    assign B0[3] = {A0[2][26:0],A0[2][31:27]} ^ {A0[2][17:0],A0[2][31:18]};

    assign B1[0] = {A1[3][26:0],A1[3][31:27]} ^ {A1[3][17:0],A1[3][31:18]};
    assign B1[1] = {A1[0][26:0],A1[0][31:27]} ^ {A1[0][17:0],A1[0][31:18]};
    assign B1[2] = {A1[1][26:0],A1[1][31:27]} ^ {A1[1][17:0],A1[1][31:18]};
    assign B1[3] = {A1[2][26:0],A1[2][31:27]} ^ {A1[2][17:0],A1[2][31:18]};

    assign I1_0[0] = B0[0] ^ Si_0[08];
    assign I1_0[1] = B0[1] ^ Si_0[09];
    assign I1_0[2] = B0[2] ^ Si_0[10];
    assign I1_0[3] = B0[3] ^ Si_0[11];

    assign C0[00]  = B0[0] ^ Si_0[00];
    assign C0[01]  = B0[1] ^ Si_0[01];
    assign C0[02]  = B0[2] ^ Si_0[02];
    assign C0[03]  = B0[3] ^ Si_0[03];
    assign C0[04]  = B0[3] ^ Si_0[07];
    assign C0[05]  = B0[0] ^ Si_0[04];
    assign C0[06]  = B0[1] ^ Si_0[05];
    assign C0[07]  = B0[2] ^ Si_0[06];
    assign C0[08]  = {I1_0[0][20:0],I1_0[0][31:21]};
    assign C0[09]  = {I1_0[1][20:0],I1_0[1][31:21]};
    assign C0[10]  = {I1_0[2][20:0],I1_0[2][31:21]};
    assign C0[11]  = {I1_0[3][20:0],I1_0[3][31:21]};

    assign I1_1[0] = B1[0] ^ Si_1[08];
    assign I1_1[1] = B1[1] ^ Si_1[09];
    assign I1_1[2] = B1[2] ^ Si_1[10];
    assign I1_1[3] = B1[3] ^ Si_1[11];

    assign C1[00]  = B1[0] ^ Si_1[00];
    assign C1[01]  = B1[1] ^ Si_1[01];
    assign C1[02]  = B1[2] ^ Si_1[02];
    assign C1[03]  = B1[3] ^ Si_1[03];
    assign C1[04]  = B1[3] ^ Si_1[07];
    assign C1[05]  = B1[0] ^ Si_1[04];
    assign C1[06]  = B1[1] ^ Si_1[05];
    assign C1[07]  = B1[2] ^ Si_1[06];
    assign C1[08]  = {I1_1[0][20:0],I1_1[0][31:21]};
    assign C1[09]  = {I1_1[1][20:0],I1_1[1][31:21]};
    assign C1[10]  = {I1_1[2][20:0],I1_1[2][31:21]};
    assign C1[11]  = {I1_1[3][20:0],I1_1[3][31:21]};

    always @(posedge clk) begin
      if(rst) begin
         for(j=0;j<12;j=j+1) begin
           D0[j] <= 32'b0;
           D1[j] <= 32'b0;
         end
      end 
      else begin
           D0[00] <= C0[00] ^ (~C0[04] & C0[08]) ^ (~C0[04] & C1[08]) ^ rs_s[00];
           D0[01] <= C0[01] ^ (~C0[05] & C0[09]) ^ (~C0[05] & C1[09]) ^ rs_s[01];
           D0[02] <= C0[02] ^ (~C0[06] & C0[10]) ^ (~C0[06] & C1[10]) ^ rs_s[02];
           D0[03] <= C0[03] ^ (~C0[07] & C0[11]) ^ (~C0[07] & C1[11]) ^ rs_s[03];
           D0[04] <= C0[04] ^ (~C0[08] & C0[00]) ^ (~C0[08] & (C1[00] ^ rconst)) ^ rs_s[04];
           D0[05] <= C0[05] ^ (~C0[09] & C0[01]) ^ (~C0[09] & C1[01]) ^ rs_s[05];
           D0[06] <= C0[06] ^ (~C0[10] & C0[02]) ^ (~C0[10] & C1[02]) ^ rs_s[06];
           D0[07] <= C0[07] ^ (~C0[11] & C0[03]) ^ (~C0[11] & C1[03]) ^ rs_s[07];
           D0[08] <= C0[08] ^ (~C0[00] & C0[04]) ^ (~C0[00] & C1[04]) ^ rs_s[08];
           D0[09] <= C0[09] ^ (~C0[01] & C0[05]) ^ (~C0[01] & C1[05]) ^ rs_s[09];
           D0[10] <= C0[10] ^ (~C0[02] & C0[06]) ^ (~C0[02] & C1[06]) ^ rs_s[10];
           D0[11] <= C0[11] ^ (~C0[03] & C0[07]) ^ (~C0[03] & C1[07]) ^ rs_s[11];
           D1[00] <=(C1[00] ^ rconst) ^ (C1[04] & C1[08]) ^ ( C1[04] & C0[08]) ^ rs_s[00];
           D1[01] <= C1[01] ^ ( C1[05] & C1[09]) ^ ( C1[05] & C0[09]) ^ rs_s[01];
           D1[02] <= C1[02] ^ ( C1[06] & C1[10]) ^ ( C1[06] & C0[10]) ^ rs_s[02];
           D1[03] <= C1[03] ^ ( C1[07] & C1[11]) ^ ( C1[07] & C0[11]) ^ rs_s[03];
           D1[04] <= C1[04] ^ ( C1[08] &(C1[00] ^ rconst)) ^ (C1[08] & C0[00]) ^ rs_s[04];
           D1[05] <= C1[05] ^ ( C1[09] & C1[01]) ^ ( C1[09] & C0[01]) ^ rs_s[05];
           D1[06] <= C1[06] ^ ( C1[10] & C1[02]) ^ ( C1[10] & C0[02]) ^ rs_s[06];
           D1[07] <= C1[07] ^ ( C1[11] & C1[03]) ^ ( C1[11] & C0[03]) ^ rs_s[07];
           D1[08] <= C1[08] ^ ((C1[00] ^ rconst) & C1[04]) ^((C1[00] ^ rconst) & C0[04]) ^ rs_s[08];
           D1[09] <= C1[09] ^ ( C1[01] & C1[05]) ^ ( C1[01] & C0[05]) ^ rs_s[09];
           D1[10] <= C1[10] ^ ( C1[02] & C1[06]) ^ ( C1[02] & C0[06]) ^ rs_s[10];
           D1[11] <= C1[11] ^ ( C1[03] & C1[07]) ^ ( C1[03] & C0[07]) ^ rs_s[11];
      end
    end

    assign So_0[00] =  D0[00];
    assign So_0[01] =  D0[01];
    assign So_0[02] =  D0[02];
    assign So_0[03] =  D0[03];
    assign So_0[04] = {D0[04][30:0],D0[04][31]};
    assign So_0[05] = {D0[05][30:0],D0[05][31]};
    assign So_0[06] = {D0[06][30:0],D0[06][31]};
    assign So_0[07] = {D0[07][30:0],D0[07][31]};
    assign So_0[08] = {D0[10][23:0],D0[10][31:24]};
    assign So_0[09] = {D0[11][23:0],D0[11][31:24]};
    assign So_0[10] = {D0[08][23:0],D0[08][31:24]};
    assign So_0[11] = {D0[09][23:0],D0[09][31:24]};

    assign So_1[00] =  D1[00];
    assign So_1[01] =  D1[01];
    assign So_1[02] =  D1[02];
    assign So_1[03] =  D1[03];
    assign So_1[04] = {D1[04][30:0],D1[04][31]};
    assign So_1[05] = {D1[05][30:0],D1[05][31]};
    assign So_1[06] = {D1[06][30:0],D1[06][31]};
    assign So_1[07] = {D1[07][30:0],D1[07][31]};
    assign So_1[08] = {D1[10][23:0],D1[10][31:24]};
    assign So_1[09] = {D1[11][23:0],D1[11][31:24]};
    assign So_1[10] = {D1[08][23:0],D1[08][31:24]};
    assign So_1[11] = {D1[09][23:0],D1[09][31:24]};

endmodule