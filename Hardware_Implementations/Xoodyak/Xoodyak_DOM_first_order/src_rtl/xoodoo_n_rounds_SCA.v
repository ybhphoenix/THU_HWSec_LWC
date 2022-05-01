/*===========================================================================*\
           Filename : xoodoo_n_rounds_SCA.v
             Author : Shuying Yin <yinshuying@mail.tsinghua.edu.cn>
        Description : N rounds of Xoodoo
          Called by : xoodoo_SCA
   Revision History : 2022-04-18, Revision 0.1.0, Shuying Yin
                      2022-04-23, Revision 0.1.1, Shuying Yin
                        Add rdi_en for rdi handshake. 
                        Remove parameter roundsPerCycle.
            License : GNU General Public License v3.0 (GPL-3.0)
                      For more information please see:
                      https://spdx.org/licenses/GPL-3.0.html
\*===========================================================================*/

module xoodoo_n_rounds_SCA
(
    input           clk            ,
    input           rst            ,
    input  [ 383:0] state_in0      ,
    input  [ 383:0] state_in1      ,
    input  [ 383:0] rdi            ,
    input           rdi_en         ,
    output [ 383:0] state_out0     ,
    output [ 383:0] state_out1     ,
    input  [  12:0] j_in           ,
    output [  12:0] j_out
);

    wire   [  31:0] rc;

    xoodoo_rconst u_rconst(.j_in(j_in),.rc(rc),.j_out(j_out));
    xoodoo_round_SCA  u_round(.clk(clk),.rst(rst),.in_0(state_in0),.in_1(state_in1),.rdi(rdi),.rdi_en(rdi_en),.rconst(rc),.out_0(state_out0),.out_1(state_out1));

endmodule

module xoodoo_rconst
(
    input  [12:0] j_in   ,
    output [31:0] rc     ,
    output [12:0] j_out
);
 
    assign rc[01] = j_in[11];
    assign rc[02] = |{j_in[7],j_in[5]};
    assign rc[03] = |{j_in[7],j_in[1:0]};
    assign rc[04] = |{j_in[11],j_in[9],j_in[5],j_in[3],j_in[1:0]};
    assign rc[05] = |{j_in[10:9],j_in[7:6],j_in[4],j_in[1]};
    assign rc[06] = |{j_in[9],j_in[6],j_in[3:2],j_in[0]};
    assign rc[07] = |{j_in[10:8],j_in[3:2]};
    assign rc[08] = |{j_in[10],j_in[8],j_in[4],j_in[2]};
    assign rc[09] = |{j_in[8],j_in[2]};

    assign {rc[31:10],rc[0]} = 23'b0;
    assign j_out = j_in<<1;

endmodule