// Manager.c — a CLI password manager, my C project.
// Stores app logins in a plain-text vault file (output/vault.txt).
// Reads it on start, rewrites it on every create. No encryption here —
// it's a learning project, don't put real passwords in it.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// One saved login: app name, username, password.
// Fixed-size char arrays keep it dead simple (no dynamic memory).
typedef struct {
    char App[32];   // e.g. "GitHub"
    char User[32];  // e.g. "Dyno1927"
    char Pass[32];  // e.g. "hunter2"
} Account;

int main(void) {
    int running = 1;          // menu loop flag; 0 = exit
    char cmd[16];             // what the user typed
    Account vault[1024];      // up to 1024 stored accounts
    int account_count = 0;    // how many slots are actually filled
    int choice;               // which account the user picked

    // --- Load the vault from disk (if it exists) ------------------
    // fopen with "r" returns NULL if there's no file yet, which is
    // fine on first run. NOTE: C's fopen never creates folders, so
    // output/ must already exist — that's why .gitkeep is in there.
    FILE* fp = fopen("output/vault.txt", "r");

    if (fp != NULL) {
        // First line = how many accounts follow. %31s caps input at
        // 31 chars so the 32-char buffers never overflow (1 spare for '\0').
        if (fscanf(fp, "%d", &account_count) == 1) {
            for (int i = 0; i < account_count; i++) {
                // App, User, Pass space-separated, one account per line
                fscanf(fp, "%31s %31s %31s", vault[i].App, vault[i].User, vault[i].Pass);
            }
        }
        fclose(fp);  // always close what I open
    }

    printf("Hello, This is a Password Manager CLI\n");

    // --- Main menu loop --------------------------------------------
    while (running) {
        printf("\nPlease Choose an option [Manage | Create | Exit]: ");

        // %15s reads one word, capped at 15 chars to fit cmd[16].
        scanf(" %15s", cmd);

        if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "e") == 0) {
            // Exit. Vault was already saved on every create, so nothing
            // to flush here — just stop the loop.
            printf("Alright, see you again!\n");
            running = 0;
        } else if (cmd[0] == 'M' || cmd[0] == 'm') {
            // --- Manage: list accounts and view one -----------------
            printf("\n-> Selected: Manage\n\n");

            if (account_count == 0) {
                printf("No accounts stored yet!\n");
            } else {
                // Print every saved app, numbered 1..n (1-indexed for humans).
                for (int i = 0; i < account_count; i++) {
                    printf("%d. %s\n", i + 1, vault[i].App);
                }

                printf("-> Please select which account to view: ");

                if (scanf("%d", &choice) == 1) {
                    int index = choice - 1;  // convert to 0-indexed array position

                    // Bounds-check BEFORE touching the array — reading past
                    // the end is undefined behavior in C and one of the
                    // most common ways to crash.
                    if (index >= 0 && index < account_count) {
                        printf("\n-> App name: %s\n", vault[index].App);
                        printf("-> Username: %s\n", vault[index].User);
                        printf("-> Password: %s\n", vault[index].Pass);
                    } else {
                        printf("Please select a valid option!\n");
                    }
                }
            }
        } else if (cmd[0] == 'C' || cmd[0] == 'c') {
            // --- Create: add a new account and save to disk ---------
            printf("\n-> Selected: Create\n");

            if (account_count < 1024) {  // guard: don't overflow the array
                printf("\n-> Please choose the apps name: ");
                scanf(" %31s", vault[account_count].App);

                printf("-> Please choose the username or email: ");
                scanf(" %31s", vault[account_count].User);

                printf("-> Please choose a password: ");
                scanf(" %31s", vault[account_count].Pass);

                // Echo the entry back so I can confirm it's what I entered.
                printf("\n-> App name: %s\n", vault[account_count].App);
                printf("-> Username: %s\n", vault[account_count].User);
                printf("-> Password: %s\n", vault[account_count].Pass);

                account_count++;  // new entry is now part of the vault

                // --- Rewrite the whole vault file -------------------
                // "w" truncates and rewrites everything, so the file
                // always mirrors what's in memory.
                FILE* save_fp = fopen("output/vault.txt", "w");
                if (save_fp != NULL) {
                    // Line 1: the count, so load knows how many to read.
                    fprintf(save_fp, "%d\n", account_count);

                    // Then one line per account, space-separated.
                    for (int i = 0; i < account_count; i++) {
                        fprintf(save_fp, "%s %s %s\n", vault[i].App, vault[i].User, vault[i].Pass);
                    }

                    fclose(save_fp);
                } else {
                    // Usually means output/ doesn't exist — fopen can't
                    // make directories, only files.
                    printf("Error: Could not save to disk.\n");
                }
            } else {
                printf("Vault is full!\n");
            }
        } else {
            printf("Invalid option. Please try again.\n");
        }
    }
    return 0;
}