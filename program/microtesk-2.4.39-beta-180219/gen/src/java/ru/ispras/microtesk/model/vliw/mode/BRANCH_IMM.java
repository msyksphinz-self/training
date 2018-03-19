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

package ru.ispras.microtesk.model.vliw.mode;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.memory.Location;
import ru.ispras.microtesk.model.vliw.PE;
import ru.ispras.microtesk.model.vliw.TempVars;

import static ru.ispras.microtesk.model.vliw.TypeDefs.*;

public final class BRANCH_IMM extends IsaPrimitive {
  public static final String NAME = "BRANCH_IMM";

  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.MODE,
          "BRANCH_IMM",
          BRANCH_IMM.class,
          SHORT
          );
      addArgument("imm", SHORT);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate imm = (Immediate) getArgument("imm", args);
      return new BRANCH_IMM(imm);
    }
  }

  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate imm;

  public BRANCH_IMM(final Immediate imm) {
    this.imm = imm;

    addArgument("imm", imm);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("0x%X", imm.access().load().bigIntegerValue(false));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s", imm.access().load().toString());
  }

  @Override
  public Location access(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    return annotate(imm.access(), vars__);
  }
}