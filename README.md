# NIST LWC SCA Protected Hardware and Software implementations

- Team: Hardware Security and Cryptographic Processor Lab, Tsinghua University, Beijing, China.
- Team Member: Leibo Liu, Wenping Zhu, Shuying Yin, Bohan Yang, Cankun Zhao, Shuohang Peng.
- Contact: Bohan Yang, bohanyang@tsinghua.edu.cn.

## Hardware Implementations

### Xoodyak

| Variant                                                      | Protection Order | Protection Method | Initial Evaluation                                           |
| ------------------------------------------------------------ | ---------------- | ----------------- | ------------------------------------------------------------ |
| [Xoodyak_DOM_first_order](https://github.com/ybhphoenix/THU_HWSec_LWC/tree/main/Hardware_Implementations/Xoodyak/Xoodyak_DOM_first_order) | 1                | DOM               | [1st-order t-test, 2nd-order t-test, $\chi^2$-test](https://github.com/ybhphoenix/THU_HWSec_LWC/blob/main/Hardware_Implementations/Xoodyak/Xoodyak_DOM_first_order/docs/documentation.pdf) |
| [Xoodyak_TI_first_order](https://github.com/ybhphoenix/THU_HWSec_LWC/tree/main/Hardware_Implementations/Xoodyak/Xoodyak_TI_first_order) | 1                | TI                | [1st-order t-test, 2nd-order t-test, $\chi^2$-test](https://github.com/ybhphoenix/THU_HWSec_LWC/blob/main/Hardware_Implementations/Xoodyak/Xoodyak_TI_first_order/docs/documentation.pdf) |

## Software Implementations

### Xoodyak

| Variant                                                      | Protection Order | Protection Method         | Initial Evaluation |
| ------------------------------------------------------------ | ---------------- | ------------------------- | ------------------ |
| [protected_Xoodyak_SCA_1st](https://github.com/ybhphoenix/THU_HWSec_LWC/tree/main/Software_Implementations/Xoodyak) | 1                | ISW Scheme, t-SNI Refresh | -                  |