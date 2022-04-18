/*===========================================================================*\
           Filename : xoodoo_n_rounds_SCA.v
             Author : Shuying Yin <yinshuying@mail.tsinghua.edu.cn>
        Description : N rounds of Xoodoo
          Called by : xoodoo_SCA
   Revision History : 2022-04-18, Revision 0.1.0, Shuying Yin
            License : GNU General Public License v3.0 (GPL-3.0)
                      For more information please see:
                      https://spdx.org/licenses/GPL-3.0.html
\*===========================================================================*/

module xoodoo_n_rounds_SCA
#(
    parameter roundPerCycle = 1 
)
(
    input           clk            ,
    input           rst            ,
    input  [ 383:0] state_in0      ,
    input  [ 383:0] state_in1      ,
    input  [ 383:0] rs0            ,
    input  [ 383:0] rs1            ,
    output [ 383:0] state_out0     ,
    output [ 383:0] state_out1     ,
    input  [  12:0] j_in           ,
    output [  12:0] j_out
);
 
    genvar i;

    wire   [  12:0] j_rc_in [0:(roundPerCycle-1)];
    wire   [  12:0] j_rc_out[0:(roundPerCycle-1)];
    wire   [  31:0] rc[0:(roundPerCycle-1)];
    wire   [ 383:0] xoodoo_state_in0 [0:(roundPerCycle-1)];
    wire   [ 383:0] xoodoo_state_in1 [0:(roundPerCycle-1)];
    wire   [ 383:0] xoodoo_state_out0[0:(roundPerCycle-1)];
    wire   [ 383:0] xoodoo_state_out1[0:(roundPerCycle-1)];

    generate
    for(i=0;i<roundPerCycle;i=i+1) begin: ALL_ROUNDS
      assign j_rc_in[i] = (i==0)? j_in : j_rc_out[i-1];
      assign xoodoo_state_in0[i] = (i==0)? state_in0 : xoodoo_state_out0[i-1];
      assign xoodoo_state_in1[i] = (i==0)? state_in1 : xoodoo_state_out1[i-1];
      xoodoo_rconst u_rconst(.clk(clk),.rst(rst),.j_in(j_rc_in[i]),.rc(rc[i]),.j_out(j_rc_out[i]));
      xoodoo_round_SCA  u_round(.clk(clk),.rst(rst),.in_0(xoodoo_state_in0[i]),.in_1(xoodoo_state_in1[i]),.rs0(rs0),.rs1(rs1),.rconst(rc[i]),.out_0(xoodoo_state_out0[i]),.out_1(xoodoo_state_out1[i]));
    end
    endgenerate

    assign j_out = j_rc_out[roundPerCycle-1];
    assign state_out0 = xoodoo_state_out0[roundPerCycle-1];
    assign state_out1 = xoodoo_state_out1[roundPerCycle-1];

endmodule

module xoodoo_rconst
(
    input                clk     ,
    input                rst     ,
    input       [  12:0] j_in    ,
    output      [  31:0] rc      ,
    output      [  12:0] j_out
);

    reg         [  31:0] rc_r    ;
    reg         [  31:0] rc_r2   ;
    reg         [  12:0] j_out_r ;
    reg         [  12:0] j_out_r2;

    always @(posedge clk) begin
      if(rst) begin
         rc_r   [31:0] <= 32'b0;
         j_out_r[12:0] <= 13'b0;
      end 
      else begin
         rc_r[01] <= j_in[11];
         rc_r[02] <= |{j_in[7],j_in[5]};
         rc_r[03] <= |{j_in[7],j_in[1:0]};
         rc_r[04] <= |{j_in[11],j_in[9],j_in[5],j_in[3],j_in[1:0]};
         rc_r[05] <= |{j_in[10:9],j_in[7:6],j_in[4],j_in[1]};
         rc_r[06] <= |{j_in[9],j_in[6],j_in[3:2],j_in[0]};
         rc_r[07] <= |{j_in[10:8],j_in[3:2]};
         rc_r[08] <= |{j_in[10],j_in[8],j_in[4],j_in[2]};
         rc_r[09] <= |{j_in[8],j_in[2]};
         {rc_r[31:10],rc_r[0]} <= 23'b0;
         j_out_r  <= j_in<<1;
      end
    end

    always @(posedge clk) begin
      if(rst) begin
         rc_r2    <= 32'b0;
         j_out_r2 <= 13'b0;
      end 
      else begin
         rc_r2    <= rc_r;
         j_out_r2 <= j_out_r;
      end
    end

    assign rc    = (rc_r==32'h58)?  32'h58 : rc_r2;
    assign j_out = (j_out_r==13'h2)? 13'h2 : j_out_r2;

endmodule
