/* SPDX-License-Identifier: GPL-2.0 */

#ifndef SEC_CCCI_LIB_H
#define SEC_CCCI_LIB_H

int masp_ccci_signfmt_verify_file(char *file_path, unsigned int *data_offset,
				  unsigned int *data_sec_len);
int masp_ccci_version_info(void);
int masp_ccci_is_cipherfmt(int fp_id, unsigned int start_off,
			   unsigned int *img_len);
int masp_ccci_decrypt_cipherfmt(int fp_id, unsigned int start_off, char *buf,
				unsigned int buf_len,
				unsigned int *data_offset);

#endif				/* SEC_CCCI_LIB_H */
