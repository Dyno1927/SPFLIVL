// Manager.rs — a CLI password manager, my Rust project.
// Stores app logins (app name, username, password) in a plain-text
// vault file (Output/Vault.txt). Reads it on start, rewrites it on
// every create. No encryption here — this is a learning project, don't
// put real passwords in it.
//
// Rust ideas this file touches:
//   - struct, deriving traits (Debug, Clone)
//   - Vec<T> (growable array) instead of a fixed C array
//   - Ownership & borrowing: functions take &str / &Account / &[Account]
//   - Option / Result for "might fail" values, plus the `?` operator
//   - match expressions and if-let patterns
//   - String (owned) vs &str (borrowed slice)
//   - println! / print! macros and stdout flushing

use std::fs;
use std::io::{self, Write};

// --- One saved login -----------------------------------------------
// `#[derive(...)]` auto-generates trait impls for us: Debug lets us
// print with {:?}, Clone lets us copy an Account cheaply.
#[derive(Debug, Clone)]
struct Account {
    app: String,  // e.g. "GitHub"
    user: String, // e.g. "Dyno1927"
    pass: String, // e.g. "hunter2"
}

const MAX_ACCOUNTS: usize = 1024;
const VAULT_PATH: &str = "Output/Vault.txt";

// --- Load the vault from disk (if it exists) -----------------------
// Returns a Vec<Account>. An empty Vec means "no file yet", which is
// fine on first run. No NULL checks like in C — the type system
// encodes "might not exist" via Result/Option instead.
fn load_vault() -> Vec<Account> {
    // read_to_string gives Result<String, io::Error>. We match on it:
    // Ok(contents) -> parse; Err(_) -> empty vault.
    let contents = match fs::read_to_string(VAULT_PATH) {
        Ok(s) => s,
        Err(_) => return Vec::new(), // first run: nothing to load
    };

    let mut lines = contents.lines();

    // First line = how many accounts follow. parse() can fail (bad file),
    // so we use and_then + ok() to fall back to an empty vault.
    let count: usize = match lines.next().and_then(|l| l.trim().parse().ok()) {
        Some(n) if n <= MAX_ACCOUNTS => n,
        _ => return Vec::new(), // bogus count -> ignore file
    };

    let mut vault = Vec::with_capacity(count);

    // Take exactly `count` lines; each is "App User Pass".
    for line in lines.take(count) {
        let mut parts = line.split_whitespace();
        // if-let destructures the Option tuple: all three must be Some.
        if let (Some(app), Some(user), Some(pass)) = (parts.next(), parts.next(), parts.next()) {
            // to_string() turns a &str slice into an owned String.
            vault.push(Account {
                app: app.to_string(),
                user: user.to_string(),
                pass: pass.to_string(),
            });
        } else {
            // Truncated file — stop reading rather than panic.
            break;
        }
    }

    vault
}

// --- Save the whole vault to disk ----------------------------------
// Takes a slice &[Account]: a borrowed view of the Vec, no copy.
// Returns io::Result<()> so callers can react to write errors.
fn save_vault(vault: &[Account]) -> io::Result<()> {
    // create_dir_all makes Output/ if missing (C's fopen can't do this).
    fs::create_dir_all("Output")?;

    // `?` propagates an error early, like returning on failure in C.
    let mut file = fs::File::create(VAULT_PATH)?;
    writeln!(file, "{}", vault.len())?;
    for acc in vault {
        writeln!(file, "{} {} {}", acc.app, acc.user, acc.pass)?;
    }
    Ok(()) // success: unit value
}

// --- Read one line of user input -----------------------------------
fn read_line(prompt: &str) -> String {
    print!("{}", prompt);
    // stdout is buffered; flush so the prompt shows before blocking.
    let _ = io::stdout().flush();

    let mut buf = String::new();
    // read_line appends into buf and returns io::Result<usize>.
    // We ignore the byte count; an empty read (EOF) just yields "".
    let _ = io::stdin().read_line(&mut buf);

    // trim() strips the trailing newline (and surrounding spaces).
    buf.trim().to_string()
}

fn main() {
    // I load on start — same as the C/C++ versions.
    let mut vault: Vec<Account> = load_vault();

    println!("Hello, this is a Password Manager CLI (in Rust).");

    // --- Main menu loop --------------------------------------------
    // `loop` is Rust's infinite loop; `break` exits it.
    loop {
        let cmd = read_line("\nPlease choose an option [Manage | Create | Exit]: ");

        // match is Rust's switch, but exhaustive and pattern-based.
        // to_lowercase() gives a temporary String; as_str() borrows it.
        match cmd.to_lowercase().as_str() {
            // Exit. Vault is saved on every create, so nothing to flush.
            "exit" | "e" => {
                println!("Alright, see you again!");
                break;
            }

            // --- Manage: list accounts and view one ---------------
            "m" | "manage" => {
                println!("\n-> Selected: Manage\n");

                if vault.is_empty() {
                    println!("No accounts stored yet!");
                } else {
                    // iter() borrows each Account; enumerate() adds an index.
                    for (i, acc) in vault.iter().enumerate() {
                        println!("{}. {}", i + 1, acc.app);
                    }

                    let choice_str = read_line("\n-> Please select which account to view: ");

                    // parse::<usize>() may fail; match on the Result.
                    match choice_str.parse::<usize>() {
                        Ok(n) if n >= 1 && n <= vault.len() => {
                            // Indexing with a known-valid n is safe here.
                            let acc = &vault[n - 1];
                            println!("\n-> App name: {}", acc.app);
                            println!("-> Username: {}", acc.user);
                            println!("-> Password: {}", acc.pass);
                        }
                        _ => println!("Please select a valid option!"),
                    }
                }
            }

            // --- Create: add a new account and save to disk -------
            "c" | "create" => {
                println!("\n-> Selected: Create");

                if vault.len() >= MAX_ACCOUNTS {
                    println!("Vault is full!");
                    continue;
                }

                let app = read_line("\n-> Please choose the app's name: ");
                let user = read_line("-> Please choose the username or email: ");
                let pass = read_line("-> Please choose a password: ");

                // Echo the entry back so I can confirm it's what I entered.
                println!("\n-> App name: {}", app);
                println!("-> Username: {}", user);
                println!("-> Password: {}", pass);

                // push moves ownership of the three Strings into the Account.
                vault.push(Account { app, user, pass });

                // --- Rewrite the whole vault file -------------------
                match save_vault(&vault) {
                    Ok(()) => println!("Account saved successfully!"),
                    Err(e) => println!("Error: Could not save to disk: {}", e),
                }
            }

            // --- Anything else -----------------------------------
            _ => println!("Invalid option. Please try again."),
        }
    }
}
