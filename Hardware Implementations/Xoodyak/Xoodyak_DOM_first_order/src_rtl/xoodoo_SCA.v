/*===========================================================================*\
           Filename : xoodoo_SCA.v
             Author : Shuying Yin <yinshuying@mail.tsinghua.edu.cn>
        Description : Xoodoo permutation
          Called by : CryptoCore_SCA
   Revision History : 2022-04-18, Revision 0.1.0, Shuying Yin
            License : GNU General Public License v3.0 (GPL-3.0)
                      For more information please see:
                      https://spdx.org/licenses/GPL-3.0.html
\*===========================================================================*/

module xoodoo_SCA
#(
    parameter roundPerCycle = 1
)
(
    input               clk_i              ,
    input               rst_i              ,
    input               start_i            ,
    output              state_valid_o      ,
    input               init_reg           ,
    input  [    383:0]  rs                 ,
    input               rdi_valid          ,
    output              rdi_ready          ,
    input  [ 32*2-1:0]  word_in            ,
    input  [      3:0]  word_index_in      ,
    input               word_enable_in     ,
    input  [     31:0]  domain_i           ,
    input               domain_enable_i    ,
    output [ 32*2-1:0]  word_out
);

    reg                 done               ;
    reg                 running            ;
    reg    [     12:0]  j_cnt_in           ;
    wire   [     12:0]  j_cnt_out          ;
    wire   [    383:0]  reg_in0            ;
    wire   [    383:0]  reg_in1            ;
    wire   [    383:0]  reg_out0           ;
    wire   [    383:0]  reg_out1           ;
    wire   [    383:0]  rs_s               ;

    reg                 j_cnt_rst          ;
    reg                 j_cnt_rst_r        ;

    always @(posedge clk_i) begin
      j_cnt_rst    <= (j_cnt_in == 13'h800)? 1'b1 : 1'b0;
      j_cnt_rst_r  <= j_cnt_rst;
    end

    always @(posedge clk_i) begin
      if(rst_i) begin
         done     <= 1'b0;
         running  <= 1'b0;
         j_cnt_in <= 13'b1;
      end
      else begin
        if(start_i) begin
           done     <= 1'b0;
           running  <= 1'b1;
           j_cnt_in <= j_cnt_out;
        end
        else if(running) begin
           done     <= 1'b0;
           running  <= 1'b1;
           j_cnt_in <= j_cnt_out; 
        end
        if(j_cnt_rst_r) begin
           done     <= 1'b1;
           running  <= 1'b0;
           j_cnt_in <= 13'b1;
        end
      end
    end
 
    assign rs_s = rdi_valid ? rs : 384'b0;

    xoodoo_register_SCA u_xoodoo_register_SCA
    (
        .clk                 (clk_i            ),
        .rst                 (rst_i            ),
        .init                (init_reg         ),
        .state_in0           (reg_in0          ),
        .state_in1           (reg_in1          ),
        .state_out0          (reg_out0         ),
        .state_out1          (reg_out1         ),
        .word_in             (word_in          ),
        .word_index_in       (word_index_in    ),
        .word_enable_in      (word_enable_in   ),
        .start_in            (start_i          ),
        .running_in          (running          ),
        .domain_i            (domain_i         ),
        .domain_enable_i     (domain_enable_i  ),
        .word_out            (word_out         )
    );

    xoodoo_n_rounds_SCA
    #(
      roundPerCycle
    ) 
    u_xoodoo_n_rounds_SCA(
        .clk                 (clk_i            ),
        .rst                 (rst_i            ),
        .state_in0           (reg_out0         ),
        .state_in1           (reg_out1         ),
        .rs                  (rs_s             ),
        .state_out0          (reg_in0          ),
        .state_out1          (reg_in1          ),
        .j_in                (j_cnt_in         ),
        .j_out               (j_cnt_out        )
    );

    assign state_valid_o = done;
    assign rdi_ready     = start_i|running ;

endmodule
