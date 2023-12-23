#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <TlHelp32.h>

int get_process();
int read_mem(LPCVOID address, LPVOID read_var, int size_data_type);
int write_mem(LPCVOID address, LPVOID write_var, int size_data_type);
void setup_offsets();

struct globals_t {
    HWND cs_process;
    int proc_id;
    DWORD proc_handle;
    uintptr_t client_module;
} globals;

struct offsets_t {
    DWORD local_player_pawn;
    DWORD f_flags;
} offsets;

//TODO: Automatic Updates, Read Data through cURL, Will be implemented later on
void setup_offsets() {
    offsets.local_player_pawn = 0x16C8F38;
    offsets.f_flags = 0x3C8;
}

int get_process() {
    globals.cs_process = FindWindow(0, "Counter-Strike 2");
    GetWindowThreadProcessId(globals.cs_process, &globals.proc_id);
    globals.proc_handle = OpenProcess(PROCESS_ALL_ACCESS, 0, globals.proc_id);
    globals.client_module = get_mod_base(globals.proc_id ,"client.dll");

    if(globals.proc_handle == 0) {
        printf("Failed to find Process!\n");
        getchar();
        get_process();
        return -1;
    }
    else {
        printf("Found Process!");
        return 1;
    }  
    return -1;
}

void main() {
    setup_offsets();
    get_process();

    while(1) {
        //Call your Modules here
    }
}

uintptr_t get_mod_base(DWORD proc_id, const wchar_t* module_name) {
    uintptr_t mod_base_add = 0;
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);
    if (h_snap != INVALID_HANDLE_VALUE) {

        MODULEENTRY32 mod_entry;
        mod_entry.dwSize = sizeof(mod_entry);

        if (Module32First(h_snap, &mod_entry)) {
            do {
                if (!strcmp(mod_entry.szModule,module_name)) {
                    mod_base_add = mod_entry.modBaseAddr;
                    break;
                }
            }
            while (Module32Next(h_snap, &mod_entry));
        }
    }
    CloseHandle(h_snap);
    return mod_base_add;
}

int read_mem(LPCVOID address, LPVOID read_var, int size_data_type) {
    return ReadProcessMemory(globals.proc_handle, address, read_var, size_data_type, 0);
}

int write_mem(LPCVOID address, LPVOID write_var, int size_data_type) {
    return WriteProcessMemory(globals.proc_handle, address, write_var, size_data_type, 0);
}
