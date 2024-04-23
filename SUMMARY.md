# TLDR;

## What is this:

Nomenclature to qualify different BF machines. We give a unique tag to each one.

Full proposal with details can be found at `README.md`.

## Format:
 
    [section1]_[field1]_[field2]..._[field<n>]~[section2]_[field1]_[field2]...


## Sections and Fields:

* **Cell-Value** `cv`
  * **Word-size:**  `[X]b` (x-bytes) / `uv` (unlimited-value)
  * **Negative:** `po`(Positives-only) / `mr` (Mirrored) / Shared `sd`  
  * **Overflow:** `oe` (Overflow-error) / `oi` (Overflow-ignore) / `or` (Overflow-rollback) 
  * **Underflow:** `ue` (Underflow-error) / `ui` (Underflow-ignore) / `ur` (Underflow-rollback) 
* **Pointer** `pt`
  * **Max-Cell:** `30k` (Default) / `[X]c` (Extend by X cells) / `uc` (unlimited-cells) 
  * **After-scope:** `ae` (After-scope-error) / `ai` (After-scope-ignore) / `ar` (After-scope-rollback) 
  * **Below-scope:** `be` ( Below-scope-error) / `bi` (Below-scope-ignore) / `br` (Below-scope-rollback) 
* **IO** `io`
  * **Read:** `dr` (Dirty-read) / `wi` (Wait-for-input ) 
  * **Write:** `ff` (Fire-and-forget) / `sw` ( Sample-then-write ) 
* **Version** `v`: Current version `[x]`

## Example:

    BF machine that has 30k cells with 8 bits words each, only works with 
    positives, returns errors when going over or under the allowed value 
    range, returns errors when trying to move the pointer above or below 
    the expected range, waits for inputs and waits before writing its output.
    
    cv_8b_po_oe_ue~pt_30k_ae_be_~io_wi_sw~v_1

## Nickname table:

As the same machine is likely to be used over-and-over again, we added the possibility of adding "shorthands" to invoke 
a machine. These names will be on the `nickname_table.csv` file.