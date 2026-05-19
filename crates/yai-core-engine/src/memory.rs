use crate::journal::Journal;

pub fn derive_memory_note(journal: &Journal) -> String {
    format!("memory:candidate records:{}", journal.count())
}
