# *BrainF Nomenclature Project*

----

----

---

## Introduction


BrainF*ck (BF) is an incredible esoteric language. It shows the power of a limited set of instructions while removing 
some of the assumptions we take for granted in most languages. That makes it whimsical and transforms the experience of 
programming in it a challenge and a game. Moreover, unlike other eso-langs, which are only able to make toy programs, 
you can make real programs in BF. 

**Clearly, the King of Toy Languages!**
    
However, that does not mean BF is perfect. I'd say that there are 2 things that limit its potential: memory restrictions 
and undefined behaviors. The former impedes BF from doing really complex programs, while the latter creates problems 
with its portability and sharing of code-solutions. And while more memory can be simply added (many custom 
implementations of BF simply overlook the memory limit), the lack of a cohesive way to classify BF code is much more 
problematic.

There are two solutions for this issue. One of them is for most people to agree on a single definition or standard. 
Maybe a "universal BF" or anything of the sort. However, that solution sounds as unlikely as boring. Unlikely because 
any "universal" definition will inevitably end up with detractors. And boring because each configuration has its own 
"tricks" that are fun to explore. 

The other solution, as you can imagine for the project you are currently reading, is to create a nomenclature for each 
"flavours" of BF-machine possible. Either a descriptive tag, a nickname, or anything to allow us to communicate those
small yet important differences. We will present a design for that in details here. In case you want to see a TLDR; 
version of this, feel free to check `SUMMARY.md` instead.


---

##  Rules 

There are an infinitude of ways to classify BF machines, most of the rules I pick for them are under the assumption that
they are going to be `good enough` for us. Here are the guidelines:

.

* **Names can be long but shouldn't be extensively long.** We are making a tag not a hash. As a rule of thumbs, it's 
  something that should be able to put in a file name and still appear complete in an `ls` command. So I'd say... no
  more than a 100 characters.


* **They should be human-readable.** Again, thinking of something that's not a hash. If a person sees the name of the 
  machine they should get an idea of what they are working with.


* Similar to the point above, **names should be somehow related to the machine they are named**. While it's possible to 
  use cheeky names, we should avoid completely unrelated things like calling the machines `tao-pai-pai`, `picollo`, 
  `garlic-jr`, `freezer`, `cell` and `majinbuu`.


* **No weird characters or emojis**. Again, the idea is that a file should be able to fully contain the name of the 
  machine, and a terminal should be able to display it.


* **We should try to stick as close to the original BF specs as possible.** While we can add some upgrades such as 
  extended memory, we shouldn't go while with things like a new set of instructions. One of the best features of BF is 
  its limitations, let's keep those in!


* **One should be able to introduce their classification in a BF code without changing any part of its behaviour.**
  Since BF naturally ignores everything that's not in its 6 characters sets as a comment, the only restriction is 
  that we can't use `-`,`+`,`>`,`<`,`,` and `.`.

----

## Proposal

The difficult thing is that after designed the nomenclature, chances are 2, 3 maybe 4 labels are going to reign supreme 
over the others. As such as I propose we do a two-way approach to tackle this issue: 

* First we generate a `specific` list of names for machines. (a.k.a. boring) Something that describes each of their 
  properties in details.

* Then we can have a secondary `nickname` table, were we keep shorthands of the BF machines defined in the previous 
  label that we are going to be used the most.

This way, we can try to capture the best of both words: something that can be used to specify everything, but also 
something that's not tedious to write over and over and over again.


### Label Properties 

Here we design a label that describes in details a BF machine. They are divided them in different sections, with ach 
section having its own fields, and then a list of possible values for that field. Take into consideration that some 
values may be dynamic. Version is a special case where the section has a value.

* Cell-Value `cv`
  * Word-size 
    * x-bytes `[X]b` - Goes up to value `2^x`
    * unlimited-value `uv` - No define limit, stops when underlying implementation breaks
  * Negative
    * Positives-only `po` - Work only with positives `max_value = word_size`
    * Mirrored `mr` - Work with a mirrored version where `min_value =  - word_size `
    * Shared `sd` - Divides the space of values into 2 sections (positive and negative), then `max_value = word_size//2` 
                    and `min_value = 1 - (word_size//2)`. 
  * Overflow
    * Overflow-error `oe` - When going above max value, raise error
    * Overflow-ignore `oi` - When trying to go above max value, do nothing
    * Overflow-rollback `or` - When trying to go above max value, go back to min value
  * Underflow
    * Underflow-error `ue` - When going below min value, raise error
    * Underflow-ignore `ui`  - When trying to go below min value, do nothing
    * Underflow-rollback `ur` - When trying to go below min value, go back to max value
* Pointer `pt`
  * Max-Cell
    * Default `30k` - Max value allowed of cells is 30k
    * Extend by X cells `[X]c` - Max value allowed of cells is `30k + (X * 1000)`
    * unlimited-cells `uc` - No define limit, stops when underlying implementation breaks
  * After-scope 
    * After-scope-error `ae` - When going above max position, raise error
    * After-scope-ignore `ai` - When trying to go above max position, do nothing
    * After-scope-rollback `ar` - When trying to go above max position, go back to position 0
  * Below-scope
    * Below-scope-error `be` - When going below position 0, raise error
    * Below-scope-ignore `bi` - When trying to go below 0 position, do nothing
    * Below-scope-rollback `br` - When trying to go below 0 position, go back to max position 
* IO `io`
  * Read
    * Dirty-read `dr` - Read the input even if it's repeated or noise 
    * Wait-for-input `wi` - Waits for input
  * Write
    * Fire-and-forget `ff` - Try to write, continue even if it failed 
    * Sample-then-write `sw` - Read the output channel, make sure it can be written before writing
* Version `v`
    * Current version `[x]` - Written in version x of nomenclature

  
### Syntax
  
With the properties already defined, we need to define a syntax to use them. Given that we already have sections and 
that no 2 fields could have the same value, we may be able to use that into our advantage. Here are the rules:

* Section are divided by a tilde.  (example `[cv]~[pt]`)
* The name of section should be at the start of that section.
* Section should appear in order: `cv -> pt -> io -> v`.
* We use an underscore as divider (`_`), with the value of each field present, no need to name the field.
* It's not possible to have 2 values of the same field. (example: `oe_oi`)

For example:


    BF machine that has 30k cells with 8 bits words each, only works with 
    positives, returns errors when going over or under the allowed value 
    range, returns errors when trying to move the pointer above or below 
    the expected range, waits for inputs and waits before writing its output.
    
    cv_8b_po_oe_ue~pt_30k_ae_be~io_wi_sw~v_1

This seems to be both short and readable. If we add the option of "overwriting" another instance,
then it might be even shorter. This may be important once we define the short-hands.

### Nickname Table

As explained on previous sections, it's expected that just a couple of BF machines will be used with any consistency, 
leaving the other parameters as a curiosity. Instead of fighting that, we can use it to make a table with abbreviations 
of commonly used BF Machines. These synonyms will be defined in `nickname_table.csv`, but the underlying idea is the 
following:

| Nickname |                    Label                    |
|:--------:|:-------------------------------------------:|
|  `atari`   | `cv_8b_po_oe_ue~pt_30k_ae_be~io_wi_sw~v_1`   |
| `genesis`  | `cv_16b_po_oe_ue~pt_30k_ae_be~io_wi_sw~v_1` |
|   `3do`    | `cv_32b_po_oe_ue~pt_30k_ae_be~io_wi_sw~v_1` |
|  `jaguar`  | `cv_64b_po_oe_ue~pt_30k_ae_be~io_wi_sw~v_1` |

These nicknames will need to be at least 3 characters long, and at lest 2 of those characters need to be non-numbers.

As such, when describing a `cv_8b_po_oe_ue~pt_30k_ae_be~io_wi_sw~v_1` one can simply use the shorthand `atari`. One 
additional thing is that it's possible to overwrite an imported nickname, so imagine you want a
`cv_8b_po_oe_ue~pt_30k_ar_br~io_wi_sw~v_1` BF Machine, which is almost the same as an `atari` but the after-scope and 
below-scope values changed. Then it becomes possible to do the following `atari~pt_ar_br`. Take into account that it is
NOT necessary to write the values of other sections or even to include every value of the current section. 

Under this definition it also becomes possible to get a `genesis` just by doing `atari~cv_16b`, which is why the nickname 
table should be kept as small as possible.

