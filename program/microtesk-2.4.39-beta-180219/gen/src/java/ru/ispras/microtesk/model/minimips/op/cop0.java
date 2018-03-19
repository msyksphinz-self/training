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

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.minimips.PE;
import ru.ispras.microtesk.model.minimips.TempVars;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.minimips.TypeDefs.*;

public final class cop0 extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "cop0",
          cop0.class,
          null
          );
      addArgument("cop_fun", Type.CARD(21));
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate cop_fun = (Immediate) getArgument("cop_fun", args);

      return new cop0(cop_fun);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "cop0",
          instruction.class,
          null
          );
      addArgument("cop_fun", Type.CARD(21));
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate cop_fun = (Immediate) getArgument("cop_fun", args);

      return new instruction(
        new cop0(
          cop_fun
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate cop_fun;

  public cop0(final Immediate cop_fun) {
    assert Type.CARD(21).equals(cop_fun.getType());

    this.cop_fun = cop_fun;

    addArgument("cop_fun", cop_fun);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("cop0 %s", cop_fun.access().load().toString());
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("01000000001%s", cop_fun.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
  }
}