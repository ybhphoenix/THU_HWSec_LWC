/*===========================================================================*\
           Filename : xoodoo_round_SCA.v
             Author : Shuying Yin <yinshuying@mail.tsinghua.edu.cn>
        Description : One round of Xoodoo
          Called by : xoodoo_n_rounds_SCA
   Revision History : 2022-04-18, Revision 0.1.0, Shuying Yin
                      2022-04-23, Revision 0.1.1, Shuying Yin
                        Add signal rdi_en for rdi handshake.
                      2022-04-24, Revision 0.2.0, Cankun Zhao
                        Correct the position of registers.
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
    input  [ 383:0] rdi        ,
    input           rdi_en     ,
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
    wire [  31:0] C100_RC    ;
    reg  [  31:0] D00  [0:11];
    reg  [  31:0] D01  [0:11];
    reg  [  31:0] D10  [0:11];
    reg  [  31:0] D11  [0:11];

    wire [  31:0] E0   [0:11];
    wire [  31:0] E1   [0:11];

    wire [  31:0] rdi_s[0:11];

    wire [  31:0] Si_0 [0:11];
    wire [  31:0] Si_1 [0:11];

    wire [  31:0] So_0 [0:11];
    wire [  31:0] So_1 [0:11];

    genvar i;

    integer j;

    generate
    for(i=0;i<12;i=i+1) begin:L0
        assign rdi_s[i] =  rdi[32*i+:32];
        assign Si_0[i]  = in_0[32*i+:32];
        assign Si_1[i]  = in_1[32*i+:32];
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

    assign C100_RC = C1[00] ^ rconst;

    always @(posedge clk) begin
      if(rst) begin
         for(j=0;j<12;j=j+1) begin
           D00[j] <= 32'b0;
           D01[j] <= 32'b0;
           D10[j] <= 32'b0;
           D11[j] <= 32'b0;
         end
      end 
      else if(rdi_en) begin
           D00[00] <= (~C0[04] & C0[08] ) ^ C0[00]   ;
           D00[01] <= (~C0[05] & C0[09] ) ^ C0[01]   ;
           D00[02] <= (~C0[06] & C0[10] ) ^ C0[02]   ;
           D00[03] <= (~C0[07] & C0[11] ) ^ C0[03]   ;
           D00[04] <= (~C0[08] & C0[00] ) ^ C0[04]   ;
           D00[05] <= (~C0[09] & C0[01] ) ^ C0[05]   ;
           D00[06] <= (~C0[10] & C0[02] ) ^ C0[06]   ;
           D00[07] <= (~C0[11] & C0[03] ) ^ C0[07]   ;
           D00[08] <= (~C0[00] & C0[04] ) ^ C0[08]   ;
           D00[09] <= (~C0[01] & C0[05] ) ^ C0[09]   ;
           D00[10] <= (~C0[02] & C0[06] ) ^ C0[10]   ;
           D00[11] <= (~C0[03] & C0[07] ) ^ C0[11]   ;
           D01[00] <= (~C0[04] & C1[08] ) ^ rdi_s[00];
           D01[01] <= (~C0[05] & C1[09] ) ^ rdi_s[01];
           D01[02] <= (~C0[06] & C1[10] ) ^ rdi_s[02];
           D01[03] <= (~C0[07] & C1[11] ) ^ rdi_s[03];
           D01[04] <= (~C0[08] & C100_RC) ^ rdi_s[04];
           D01[05] <= (~C0[09] & C1[01] ) ^ rdi_s[05];
           D01[06] <= (~C0[10] & C1[02] ) ^ rdi_s[06];
           D01[07] <= (~C0[11] & C1[03] ) ^ rdi_s[07];
           D01[08] <= (~C0[00] & C1[04] ) ^ rdi_s[08];
           D01[09] <= (~C0[01] & C1[05] ) ^ rdi_s[09];
           D01[10] <= (~C0[02] & C1[06] ) ^ rdi_s[10];
           D01[11] <= (~C0[03] & C1[07] ) ^ rdi_s[11];
           D10[00] <= ( C1[04] & C1[08] ) ^ C100_RC  ;
           D10[01] <= ( C1[05] & C1[09] ) ^ C1[01]   ;
           D10[02] <= ( C1[06] & C1[10] ) ^ C1[02]   ;
           D10[03] <= ( C1[07] & C1[11] ) ^ C1[03]   ;
           D10[04] <= ( C1[08] &C100_RC ) ^ C1[04]   ;
           D10[05] <= ( C1[09] & C1[01] ) ^ C1[05]   ;
           D10[06] <= ( C1[10] & C1[02] ) ^ C1[06]   ;
           D10[07] <= ( C1[11] & C1[03] ) ^ C1[07]   ;
           D10[08] <= (C100_RC & C1[04] ) ^ C1[08]   ;
           D10[09] <= ( C1[01] & C1[05] ) ^ C1[09]   ;
           D10[10] <= ( C1[02] & C1[06] ) ^ C1[10]   ;
           D10[11] <= ( C1[03] & C1[07] ) ^ C1[11]   ;
           D11[00] <= ( C1[04] & C0[08] ) ^ rdi_s[00];
           D11[01] <= ( C1[05] & C0[09] ) ^ rdi_s[01];
           D11[02] <= ( C1[06] & C0[10] ) ^ rdi_s[02];
           D11[03] <= ( C1[07] & C0[11] ) ^ rdi_s[03];
           D11[04] <= ( C1[08] & C0[00] ) ^ rdi_s[04];
           D11[05] <= ( C1[09] & C0[01] ) ^ rdi_s[05];
           D11[06] <= ( C1[10] & C0[02] ) ^ rdi_s[06];
           D11[07] <= ( C1[11] & C0[03] ) ^ rdi_s[07];
           D11[08] <= (C100_RC & C0[04] ) ^ rdi_s[08];
           D11[09] <= ( C1[01] & C0[05] ) ^ rdi_s[09];
           D11[10] <= ( C1[02] & C0[06] ) ^ rdi_s[10];
           D11[11] <= ( C1[03] & C0[07] ) ^ rdi_s[11];
      end
    end

    assign E0[00] = D00[00] ^ D01[00];
    assign E0[01] = D00[01] ^ D01[01];
    assign E0[02] = D00[02] ^ D01[02];
    assign E0[03] = D00[03] ^ D01[03];
    assign E0[04] = D00[04] ^ D01[04];
    assign E0[05] = D00[05] ^ D01[05];
    assign E0[06] = D00[06] ^ D01[06];
    assign E0[07] = D00[07] ^ D01[07];
    assign E0[08] = D00[08] ^ D01[08];
    assign E0[09] = D00[09] ^ D01[09];
    assign E0[10] = D00[10] ^ D01[10];
    assign E0[11] = D00[11] ^ D01[11];

    assign E1[00] = D10[00] ^ D11[00];
    assign E1[01] = D10[01] ^ D11[01];
    assign E1[02] = D10[02] ^ D11[02];
    assign E1[03] = D10[03] ^ D11[03];
    assign E1[04] = D10[04] ^ D11[04];
    assign E1[05] = D10[05] ^ D11[05];
    assign E1[06] = D10[06] ^ D11[06];
    assign E1[07] = D10[07] ^ D11[07];
    assign E1[08] = D10[08] ^ D11[08];
    assign E1[09] = D10[09] ^ D11[09];
    assign E1[10] = D10[10] ^ D11[10];
    assign E1[11] = D10[11] ^ D11[11];

    assign So_0[00] =  E0[00];
    assign So_0[01] =  E0[01];
    assign So_0[02] =  E0[02];
    assign So_0[03] =  E0[03];
    assign So_0[04] = {E0[04][30:0],E0[04][31]};
    assign So_0[05] = {E0[05][30:0],E0[05][31]};
    assign So_0[06] = {E0[06][30:0],E0[06][31]};
    assign So_0[07] = {E0[07][30:0],E0[07][31]};
    assign So_0[08] = {E0[10][23:0],E0[10][31:24]};
    assign So_0[09] = {E0[11][23:0],E0[11][31:24]};
    assign So_0[10] = {E0[08][23:0],E0[08][31:24]};
    assign So_0[11] = {E0[09][23:0],E0[09][31:24]};

    assign So_1[00] =  E1[00];
    assign So_1[01] =  E1[01];
    assign So_1[02] =  E1[02];
    assign So_1[03] =  E1[03];
    assign So_1[04] = {E1[04][30:0],E1[04][31]};
    assign So_1[05] = {E1[05][30:0],E1[05][31]};
    assign So_1[06] = {E1[06][30:0],E1[06][31]};
    assign So_1[07] = {E1[07][30:0],E1[07][31]};
    assign So_1[08] = {E1[10][23:0],E1[10][31:24]};
    assign So_1[09] = {E1[11][23:0],E1[11][31:24]};
    assign So_1[10] = {E1[08][23:0],E1[08][31:24]};
    assign So_1[11] = {E1[09][23:0],E1[09][31:24]};

endmodule