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

package ru.ispras.microtesk.model.vliw.op;

import ru.ispras.microtesk.model.*;

public abstract class FP extends IsaPrimitive {
  private FP() {}

  public static final String NAME = "FP";

  public static final IsaPrimitiveInfoOr INFO = new IsaPrimitiveInfoOr(
      IsaPrimitiveKind.OP,
      NAME,
      ADD_S.INFO,
      SUB_S.INFO,
      MUL_S.INFO,
      NEG_S.INFO,
      ABS_S.INFO,
      C_EQ_S.INFO,
      C_LT_S.INFO,
      C_LE_S.INFO,
      MOV_S.INFO,
      MTF.INFO,
      MFF.INFO,
      MOVT_S.INFO,
      MOVF_S.INFO,
      MOVN_S.INFO,
      MOVZ_S.INFO
  );
} 