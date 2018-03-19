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

public abstract class test extends IsaPrimitive {
  private test() {}

  public static final String NAME = "test";

  public static final IsaPrimitiveInfoOr INFO = new IsaPrimitiveInfoOr(
      IsaPrimitiveKind.OP,
      NAME,
      test_r8r8.INFO,
      test_m8r8.INFO,
      test_r8i8.INFO,
      test_m8i8.INFO,
      test_r16r16.INFO,
      test_m16r16.INFO,
      test_r16i16.INFO,
      test_m16i16.INFO
  );
} 