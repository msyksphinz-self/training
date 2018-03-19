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

package ru.ispras.microtesk.model.minimips.mode;

import ru.ispras.microtesk.model.*;

public abstract class JUMP_ADDR extends IsaPrimitive {
  private JUMP_ADDR() {}

  public static final String NAME = "JUMP_ADDR";

  public static final IsaPrimitiveInfoOr INFO = new IsaPrimitiveInfoOr(
      IsaPrimitiveKind.MODE,
      NAME,
      JUMP_LABEL.INFO,
      JUMP_IMM.INFO
  ); 
} 