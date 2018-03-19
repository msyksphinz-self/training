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

package ru.ispras.microtesk.model.cpu.op;

import ru.ispras.microtesk.model.*;

public abstract class instruction extends IsaPrimitive {
  private instruction() {}

  public static final String NAME = "instruction";

  public static final IsaPrimitiveInfoOr INFO = new IsaPrimitiveInfoOr(
      IsaPrimitiveKind.OP,
      NAME,
      alu_instr.INFO,
      branch_instr.INFO
  );
} 