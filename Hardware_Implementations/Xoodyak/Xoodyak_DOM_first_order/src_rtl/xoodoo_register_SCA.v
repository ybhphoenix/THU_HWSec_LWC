module xoodoo_register_SCA
(
    input               clk                ,
    input               rst                ,
    input               init               ,
    input  [     383:0] state_in0          ,
    input  [     383:0] state_in1          ,
    output [     383:0] state_out0         ,
    output [     383:0] state_out1         ,
    input  [  32*2-1:0] word_in            ,
    input  [       3:0] word_index_in      ,
    input               word_enable_in     ,
    input               reg_en             ,
    input  [      31:0] domain_i           ,
    input               domain_enable_i    ,
    output [  32*2-1:0] word_out
);
    reg    [      31:0] reg_value0 [0:11]  ;
    reg    [      31:0] reg_value1 [0:11]  ;

    genvar i;

    integer j;

    generate
    for(i=0;i<12;i=i+1) begin: STATE_OUT
        assign state_out0[32*i+:32] = reg_value0[i];
        assign state_out1[32*i+:32] = reg_value1[i];
    end
    endgenerate

    always @(posedge clk) begin
      if(rst) begin
         for(j=0;j<12;j=j+1) begin
             reg_value0[j] <= 32'b0;
             reg_value1[j] <= 32'b0;
         end
      end
      else begin 
         if(init) begin
            for(j=0;j<12;j=j+1) begin
                reg_value0[j] <= 32'b0;
                reg_value1[j] <= 32'b0;
            end
         end
         else if(reg_en) begin
            for(j=0;j<12;j=j+1) begin
                reg_value0[j] <= state_in0[32*j+:32];
                reg_value1[j] <= state_in1[32*j+:32];
            end
         end
         else begin
            if(domain_enable_i) begin
               reg_value0[11] <= reg_value0[11];
               reg_value1[11] <= reg_value1[11] ^ domain_i;
            end
            if(word_enable_in) begin
               reg_value0[word_index_in] <= reg_value0[word_index_in] ^ word_in[63:32];
               reg_value1[word_index_in] <= reg_value1[word_index_in] ^ word_in[31:0];
            end
         end
      end
    end

    assign word_out = {reg_value0[word_index_in],reg_value1[word_index_in]};

endmodule