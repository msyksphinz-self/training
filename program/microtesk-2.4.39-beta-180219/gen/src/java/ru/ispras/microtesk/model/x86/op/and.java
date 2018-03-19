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

public abstract class and extends IsaPrimitive {
  private and() {}

  public static final String NAME = "and";

  public static final IsaPrimitiveInfoOr INFO = new IsaPrimitiveInfoOr(
      IsaPrimitiveKind.OP,
      NAME,
      and_r8r8.INFO,
      and_m8r8.INFO,
      and_r8m8.INFO,
      and_r8i8.INFO,
      and_m8i8.INFO,
      and_r16r16.INFO,
      and_m16r16.INFO,
      and_r16m16.INFO,
      and_r16i16.INFO,
      and_m16i16.INFO
  );
} 