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

public final class F extends IsaPrimitive {
  public static final String NAME = "F";

  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.MODE,
          "F",
          F.class,
          SINGLE
          );
      addArgument("i", INDEX);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate i = (Immediate) getArgument("i", args);
      return new F(i);
    }
  }

  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate i;

  public F(final Immediate i) {
    this.i = i;

    addArgument("i", i);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%d", i.access().load().bigIntegerValue());
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%5s", i.access().load().toString());
  }

  @Override
  public Location access(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    return annotate(pe__.FPR.access(i.access().load()), vars__);
  }
}