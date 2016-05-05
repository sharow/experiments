/* -*- Mode: c; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*- */

#ifndef __PARTIAL_H__
#define __PARTIAL_H__

void *partial(void *function, void *first_arg);
void unpartial(void *partialized);

#endif

