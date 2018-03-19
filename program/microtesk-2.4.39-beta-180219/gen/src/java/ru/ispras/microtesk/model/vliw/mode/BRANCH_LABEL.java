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

public final class BRANCH_LABEL extends IsaPrimitive {
  public static final String NAME = "BRANCH_LABEL";

  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.MODE,
          "BRANCH_LABEL",
          BRANCH_LABEL.class,
          Type.INT(16)
          );
      addArgument("target", WORD);
      addArgument("current", WORD);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate target = (Immediate) getArgument("target", args);
      final Immediate current = (Immediate) getArgument("current", args);
      return new BRANCH_LABEL(target, current);
    }
  }

  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate target;
  public final Immediate current;

  public BRANCH_LABEL(final Immediate target, final Immediate current) {
    this.target = target;
    this.current = current;

    addArgument("target", target);
    addArgument("current", current);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("");
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s", target.access().bitField(18, 3).load().subtract(current.access().bitField(18, 3).load()).subtract(Data.valueOf(Type.INT(16), 0x1)).toString());
  }

  @Override
  public Location access(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    return annotate(new Location(target.access().bitField(18, 3).load().subtract(current.access().bitField(18, 3).load()).subtract(Data.valueOf(Type.INT(16), 0x1))), vars__);
  }
}