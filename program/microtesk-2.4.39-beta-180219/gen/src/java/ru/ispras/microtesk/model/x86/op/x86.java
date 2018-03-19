/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.x86.op;

import ru.ispras.microtesk.model.*;

public abstract class x86 extends IsaPrimitive {
  private x86() {}

  public static final String NAME = "x86";

  public static final IsaPrimitiveInfoOr INFO = new IsaPrimitiveInfoOr(
      IsaPrimitiveKind.OP,
      NAME,
      mov.INFO,
      xchg.INFO,
      add.INFO,
      sub.INFO,
      push.INFO,
      pop.INFO,
      mul.INFO,
      imul.INFO,
      div.INFO,
      idiv.INFO,
      pushA.INFO,
      popA.INFO,
      adc.INFO,
      sbb.INFO,
      inc.INFO,
      dec.INFO,
      test.INFO,
      and.INFO,
      xor.INFO,
      or.INFO,
      not.INFO,
      neg.INFO,
      cmp.INFO,
      jmp.INFO,
      jo.INFO,
      jno.INFO,
      jc.INFO,
      jnc.INFO,
      jz.INFO,
      jnz.INFO,
      js.INFO,
      jns.INFO,
      jp.INFO,
      jpe.INFO,
      jnp.INFO,
      jpo.INFO,
      ja.INFO,
      jae.INFO,
      jb.INFO,
      jbe.INFO,
      je.INFO,
      jne.INFO,
      jna.INFO,
      jnae.INFO,
      jnb.INFO,
      jnbe.INFO,
      jg.INFO,
      jge.INFO,
      jl.INFO,
      jle.INFO,
      jng.INFO,
      jnge.INFO,
      jnl.INFO,
      jnle.INFO,
      int_.INFO,
      lea.INFO
  );
} 