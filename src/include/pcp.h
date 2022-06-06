#ifndef PCP_H
#define PCP_H

void pcp_compile(char* src);
void pcp_compile_file(const char* filename);
char* pcp_read_file_mmap(const char* filename);

#endif // !PCP_H
