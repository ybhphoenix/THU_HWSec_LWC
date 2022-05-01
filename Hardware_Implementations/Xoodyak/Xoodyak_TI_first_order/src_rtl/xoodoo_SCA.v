/*===========================================================================*\
           Filename : xoodoo_SCA.v
             Author : Shuying Yin <yinshuying@mail.tsinghua.edu.cn>
        Description : Xoodoo permutation
          Called by : CryptoCore_SCA
   Revision History : 2022-04-18, Revision 0.1.0, Shuying Yin
                      2022-04-23, Revision 0.1.1, Shuying Yin
                        Fix the handshake of rdi.
                        Change the port width of rdi from 768 bits to 384 bits.
                        Remove parameter roundsPerCycle.
                      2022-04-30, Revision 0.1.2, Shuying Yin
                        Add signal reg_en to bypass xoodoo_register_SCA when
                        running permutation.
            License : GNU General Public License v3.0 (GPL-3.0)
                      For more information please see:
                      https://spdx.org/licenses/GPL-3.0.html
\*===========================================================================*/

module xoodoo_SCA
(
    input               clk_i              ,
    input               rst_i              ,
    input               n_start_i          ,
    input               start_i            ,
    output              state_valid_o      ,
    input               init_reg           ,
    input  [     383:0] rdi                ,
    input               rdi_valid          ,
    output       reg    rdi_ready          ,
    input  [  32*2-1:0] word_in            ,
    input  [       3:0] word_index_in      ,
    input               word_enable_in     ,
    input  [      31:0] domain_i           ,
    input               domain_enable_i    ,
    output [  32*2-1:0] word_out
);

    reg                 done               ;
    reg                 running            ;
    reg    [      12:0] j_cnt_in           ;
    wire   [      12:0] j_cnt_out          ;
    reg                 j_cnt_clr          ;

    wire   [     383:0] reg_in0            ;
    wire   [     383:0] reg_in1            ;
    wire   [     383:0] reg_out0           ;
    wire   [     383:0] reg_out1           ;

    wire   [     383:0] st_in0             ;
    wire   [     383:0] st_in1             ;

    reg    [       1:0] rdi_cnt            ;
    wire                rdi0_en            ;
    wire                rdi1_en            ;
    reg                 rdi1_en_r          ;
    reg                 reg_en             ;

    always @(posedge clk_i) begin
      if(rst_i)
         rdi_cnt <= 2'b0; 
      else if(rdi_valid&rdi_ready)
         rdi_cnt <= rdi_cnt + 2'b1;
      else if(rdi_cnt == 2'b10)
         rdi_cnt <= 2'b0;
    end

    assign rdi0_en = ~rdi_cnt[0]&rdi_valid&rdi_ready;
    assign rdi1_en =  rdi_cnt[0]&rdi_valid&rdi_ready;

    always @(posedge clk_i) begin
      if(rst_i) begin
         rdi1_en_r   <= 1'b0;
         j_cnt_clr   <= 1'b0;
         reg_en      <= 1'b0;
      end
      else begin
         rdi1_en_r   <= rdi1_en;
         j_cnt_clr   <= ((j_cnt_in == 13'h1000) && rdi1_en_r)? 1'b1 : 1'b0;
         reg_en      <= rdi1_en_r;
      end
    end

    always @(posedge clk_i) begin
      if(rst_i)
         rdi_ready <= 1'b0;
      else if(n_start_i)
         rdi_ready <= 1'b1;
      else if(j_cnt_in == 13'h1000)
         rdi_ready <= 1'b0;
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
           j_cnt_in <= (rdi1_en)? j_cnt_out : j_cnt_in;
        end
        else if(running) begin
           done     <= 1'b0;
           running  <= 1'b1;
           j_cnt_in <= (rdi1_en)? j_cnt_out : j_cnt_in; 
        end
        if(j_cnt_clr) begin
           done     <= 1'b1;
           running  <= 1'b0;
           j_cnt_in <= 13'b1;
        end
      end
    end
 
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
        .reg_en              (reg_en           ),
        .domain_i            (domain_i         ),
        .domain_enable_i     (domain_enable_i  ),
        .word_out            (word_out         )
    );

    assign st_in0 = reg_en? reg_in0 : reg_out0;
    assign st_in1 = reg_en? reg_in1 : reg_out1;

    xoodoo_n_rounds_SCA u_xoodoo_n_rounds_SCA(
        .clk                 (clk_i            ),
        .rst                 (rst_i            ),
        .state_in0           (st_in0           ),
        .state_in1           (st_in1           ),
        .rdi                 (rdi              ),
        .rdi0_en             (rdi0_en          ),
        .rdi1_en             (rdi1_en          ),
        .state_out0          (reg_in0          ),
        .state_out1          (reg_in1          ),
        .j_in                (j_cnt_in         ),
        .j_out               (j_cnt_out        )
    );

    assign state_valid_o = done;

endmodule