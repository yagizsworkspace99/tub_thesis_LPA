// This is a C program which uses some basic features of the C wrapper around
// libcuckoo.  The C wrapper includes nearly all the features of the C++
// version, and most of the C functions have direct equivalents to C++ methods.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includes the interface for the int->string table
#include "int_str_table.h"
// Includes the interface for the key_blob->mapped_blob table.  Since both
// interface files define the same three macros, we must first undefine them
// before including the next interface. This undefining will have no effect on
// the names generated by the interface templates.
#undef CUCKOO_TABLE_NAME
#undef CUCKOO_KEY_TYPE
#undef CUCKOO_MAPPED_TYPE
#include "blob_blob_table.h"

int main() {
  // Allocate a new int->str table, which must be freed at the end of the
  // program
  int_str_table *tbl = int_str_table_init(0);
  // Since the insert function only accepts a pointer to the key and mapped
  // values, we must make sure they exist as lvalues
  const char *insert_item = "hello";
  for (int i = 0; i < 10; i++) {
    int_str_table_insert(tbl, &i, &insert_item);
  }

  // This loop will search for keys 1-10 (of which 10 will not be found), and
  // print out what it finds
  printf("int -> str table:\n");
  for (int i = 0; i < 11; i++) {
    // We need to store the found value in some variable
    const char *find_item;
    // The find function will return true if the key was found, and false
    // otherwise
    if (int_str_table_find(tbl, &i, &find_item)) {
      printf("%d  %s\n", i, find_item);
    } else {
      printf("%d  NOT FOUND\n", i);
    }
  }

  // Allocate a new blob->blob table, which must be freed at the end of the
  // program
  blob_blob_table *tbl2 = blob_blob_table_init(0);
  // The table functions as a fully-functional single-threaded hashtable in
  // locked_table mode, so we lock the table now in order to showcase some of
  // that functionality.
  blob_blob_table_locked_table *ltbl = blob_blob_table_lock_table(tbl2);
  key_blob k;
  mapped_blob v;
  for (int i = 0; i < 10; ++i) {
    // The key we store is a binary representation of the integer
    memcpy(k.blob, &i, sizeof(i));
    // The mapped value we store is the integer converted to ascii characters
    int num = i;
    int j = 0;
    if (num == 0) {
      v.blob[j++] = '0';
    } else {
      while (num > 0) {
        v.blob[j++] = (num % 10) + '0';
        num /= 10;
      }
    }
    v.blob[j] = '\0';
    // To insert into a locked_table, we pass the table, address of key and
    // value, and, optionally, an iterator, which will be set to the location
    // of the inserted key-value pair. We pass NULL here, indicating we don't
    // have an iterator to set to the insert location.
    blob_blob_table_locked_table_insert(ltbl, &k, &v, NULL);
  }

  // We must allocate iterator objects for the beginning and end of the table
  // to iterate through it. Since we are not modifying the elements of the
  // table, we use const_iterators, though regular iterators would also work
  // here.
  blob_blob_table_const_iterator *it =
      blob_blob_table_locked_table_cbegin(ltbl);
  blob_blob_table_const_iterator *end = blob_blob_table_locked_table_cend(ltbl);

  // This loop walks through the table and prints out each key-value pair.
  // Since `it` is already set to the beginning, there is no need for an
  // initialization statement. The condition tests whether `it` equals `end`,
  // which, when it does, will mean we've moved past the end of the table. The
  // step statement advances `it` forward to the next key-value pair.
  printf("blob -> blob table:\n");
  for (; !blob_blob_table_const_iterator_equal(it, end);
       blob_blob_table_const_iterator_increment(it)) {
    // Here we dereference the key and mapped values from the iterator and
    // store them into our previously-declared key_blob and mapped_blob
    // variables.
    k = *blob_blob_table_const_iterator_key(it);
    v = *blob_blob_table_const_iterator_mapped(it);
    printf("%d -> %s\n", *(int *)k.blob, v.blob);
  }

  // We must free all the allocated objects, which are the iterators, locked
  // table, and two table objects.
  blob_blob_table_const_iterator_free(end);
  blob_blob_table_const_iterator_free(it);
  blob_blob_table_locked_table_free(ltbl);
  blob_blob_table_free(tbl2);
  int_str_table_free(tbl);
}
