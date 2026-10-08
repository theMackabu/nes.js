unsigned char arr_new(unsigned int len) {
  struct arr *a;
  if (len > 255) fail("ARRAY TOO BIG");
  if (narr >= MAXA) fail("TOO MANY ARRAYS");
  if (heaptop + len > HEAPN) fail("OUT OF ARRAY MEMORY");
  a = &arrs[narr];
  a->start = heaptop;
  a->len = a->cap = len;
  memset(heap + heaptop, 0, len * 2);
  heaptop += len;
  return narr++;
}

void arr_push(unsigned char h, int val) {
  struct arr *a = &arrs[h];
  unsigned char nc;
  if (a->len == a->cap) {
    if (a->cap > 247) fail("ARRAY TOO BIG");
    nc = a->cap + 8;
    if (a->start + a->cap == heaptop && heaptop + 8 <= HEAPN) {
      heaptop += 8;
    } else {
      if (heaptop + nc > HEAPN) fail("OUT OF ARRAY MEMORY");
      memcpy(heap + heaptop, heap + a->start, a->len * 2);
      a->start = heaptop;
      heaptop += nc;
    }
    a->cap = nc;
  }
  heap[a->start + a->len++] = val;
}

int *arr_slot(unsigned char h, int idx) {
  struct arr *a = &arrs[h];
  if (idx < 0) fail("BAD INDEX");
  while (idx >= a->len)
    arr_push(h, 0);
  return &heap[a->start + idx];
}
