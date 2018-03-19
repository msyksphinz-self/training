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

package ru.ispras.microtesk.model.minimips.op;

import ru.ispras.microtesk.model.*;

public abstract class Operations extends IsaPrimitive {
  private Operations() {}

  public static final String NAME = "Operations";

  public static final IsaPrimitiveInfoOr INFO = new IsaPrimitiveInfoOr(
      IsaPrimitiveKind.OP,
      NAME,
      add.INFO,
      addi.INFO,
      addiu.INFO,
      addu.INFO,
      and.INFO,
      andi.INFO,
      beq.INFO,
      bgez.INFO,
      bgezal.INFO,
      bgtz.INFO,
      blez.INFO,
      bltz.INFO,
      bltzal.INFO,
      bne.INFO,
      mips_break.INFO,
      cop0.INFO,
      j.INFO,
      jal.INFO,
      jalr.INFO,
      jr.INFO,
      lui.INFO,
      lw.INFO,
      lwc0.INFO,
      mfc0.INFO,
      mfhi.INFO,
      mflo.INFO,
      mtc0.INFO,
      mthi.INFO,
      mtlo.INFO,
      mult.INFO,
      multu.INFO,
      nor.INFO,
      or.INFO,
      ori.INFO,
      sll.INFO,
      sllv.INFO,
      slt.INFO,
      slti.INFO,
      sltiu.INFO,
      sltu.INFO,
      sra.INFO,
      srav.INFO,
      srl.INFO,
      srlv.INFO,
      sub.INFO,
      subu.INFO,
      sw.INFO,
      swc0.INFO,
      syscall.INFO,
      xor.INFO,
      xori.INFO,
      nop.INFO
  );
} 