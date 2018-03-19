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

package ru.ispras.microtesk.model.x86.mode;

import ru.ispras.microtesk.model.*;

public abstract class MOFFS extends IsaPrimitive {
  private MOFFS() {}

  public static final String NAME = "MOFFS";

  public static final IsaPrimitiveInfoOr INFO = new IsaPrimitiveInfoOr(
      IsaPrimitiveKind.MODE,
      NAME,
      RIAM_BX.INFO,
      BIAM_BX_SI.INFO,
      IAM_BX_D8.INFO,
      BIDAM_BX_SI_D8.INFO,
      IAM_BX_D16.INFO,
      BIDAM_BX_SI_D16.INFO,
      RIAM_SI.INFO,
      BIAM_BX_DI.INFO,
      IAM_BP_D8.INFO,
      BIDAM_BX_DI_D8.INFO,
      IAM_BP_D16.INFO,
      BIDAM_BX_DI_D16.INFO,
      RIAM_DI.INFO,
      BIAM_BP_SI.INFO,
      IAM_SI_D8.INFO,
      BIDAM_BP_SI_D8.INFO,
      IAM_SI_D16.INFO,
      BIDAM_BP_SI_D16.INFO,
      DOAM.INFO,
      BIAM_BP_DI.INFO,
      IAM_DI_D8.INFO,
      BIDAM_BP_DI_D8.INFO,
      IAM_DI_D16.INFO,
      BIDAM_BP_DI_D16.INFO
  ); 
} 