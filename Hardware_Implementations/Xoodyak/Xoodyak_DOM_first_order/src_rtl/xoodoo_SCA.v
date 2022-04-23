module xoodoo_SCA
(
    input               clk_i              ,
    input               rst_i              ,
    input               n_start_i          ,
    input               start_i            ,
    output              state_valid_o      ,
    input               init_reg           ,
    input  [    383:0]  rdi                ,
    input               rdi_valid          ,
    output      reg     rdi_ready          ,
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

    wire                rdi_en             ;
    reg                 reg_en             ;

    assign rdi_en = rdi_valid&rdi_ready;

    always @(posedge clk_i) begin
      if(rst_i)
         reg_en <= 1'b0;
      else
         reg_en <= rdi_en;
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
           j_cnt_in <= (rdi_en)? j_cnt_out : j_cnt_in;
        end
        else if(running) begin
           done     <= 1'b0;
           running  <= 1'b1;
           j_cnt_in <= (rdi_en)? j_cnt_out : j_cnt_in; 
        end
        if((j_cnt_in == 13'h1000)&reg_en) begin
           done     <= 1'b1;
           running  <= 1'b0;
           j_cnt_in <= 13'b1;
        end
      end
    end
 
    always @(posedge clk_i) begin
      if(rst_i)
         rdi_ready <= 1'b0;
      else if(n_start_i)
         rdi_ready <= 1'b1;
      else if(j_cnt_in == 13'h1000)
         rdi_ready <= 1'b0;
      else if(rdi_en)
         rdi_ready <= 1'b0;
      else if(~rdi_ready&(j_cnt_in != 13'h1))
         rdi_ready <= 1'b1;
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

    xoodoo_n_rounds_SCA u_xoodoo_n_rounds_SCA(
        .clk                 (clk_i            ),
        .rst                 (rst_i            ),
        .state_in0           (reg_out0         ),
        .state_in1           (reg_out1         ),
        .rdi                 (rdi              ),
        .rdi_en              (rdi_en           ),
        .state_out0          (reg_in0          ),
        .state_out1          (reg_in1          ),
        .j_in                (j_cnt_in         ),
        .j_out               (j_cnt_out        )
    );
   
    assign state_valid_o = done;

endmodule