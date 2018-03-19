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
import ru.ispras.microtesk.model.minimips.mode.*;

import static ru.ispras.microtesk.model.minimips.TypeDefs.*;

public final class mtc0 extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "mtc0",
          mtc0.class,
          null
          );
      addArgument("rt", REG.INFO);
      addArgument("cs", RCOP0.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rt = (REG) getArgument("rt", args);
      final RCOP0 cs = (RCOP0) getArgument("cs", args);

      return new mtc0(rt, cs);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "mtc0",
          instruction.class,
          null
          );
      addArgument("rt", REG.INFO);
      addArgument("cs", RCOP0.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rt = (REG) getArgument("rt", args);
      final RCOP0 cs = (RCOP0) getArgument("cs", args);

      return new instruction(
        new mtc0(
          rt,
          cs
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rt;
  public final RCOP0 cs;

  public mtc0(final REG rt, final RCOP0 cs) {
    assert REG.INFO.isSupported(rt);
    assert RCOP0.INFO.isSupported(cs);

    this.rt = rt;
    this.cs = cs;

    addArgument("rt", rt);
    addArgument("cs", cs);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("mtc0 %s, %s", rt.text(vars__), cs.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("01000000100%s%s00000000000", rt.image(vars__), cs.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    cs.access(pe__, vars__).store(rt.access(pe__, vars__).load());
  }
}