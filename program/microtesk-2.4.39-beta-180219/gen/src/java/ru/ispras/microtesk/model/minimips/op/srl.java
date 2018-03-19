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
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.minimips.TypeDefs.*;

public final class srl extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "srl",
          srl.class,
          null
          );
      addArgument("rd", REG.INFO);
      addArgument("rt", REG.INFO);
      addArgument("sa", Type.CARD(5));
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rd = (REG) getArgument("rd", args);
      final REG rt = (REG) getArgument("rt", args);
      final Immediate sa = (Immediate) getArgument("sa", args);

      return new srl(rd, rt, sa);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "srl",
          instruction.class,
          null
          );
      addArgument("rd", REG.INFO);
      addArgument("rt", REG.INFO);
      addArgument("sa", Type.CARD(5));
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rd = (REG) getArgument("rd", args);
      final REG rt = (REG) getArgument("rt", args);
      final Immediate sa = (Immediate) getArgument("sa", args);

      return new instruction(
        new srl(
          rd,
          rt,
          sa
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rd;
  public final REG rt;
  public final Immediate sa;

  public srl(final REG rd, final REG rt, final Immediate sa) {
    assert REG.INFO.isSupported(rd);
    assert REG.INFO.isSupported(rt);
    assert Type.CARD(5).equals(sa.getType());

    this.rd = rd;
    this.rt = rt;
    this.sa = sa;

    addArgument("rd", rd);
    addArgument("rt", rt);
    addArgument("sa", sa);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("srl %s, %s, %d", rd.text(vars__), rt.text(vars__), sa.access().load().bigIntegerValue());
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("00000000000%s%s%s000010", rt.image(vars__), rd.image(vars__), sa.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    rd.access(pe__, vars__).store(Data.cast(Type.CARD(32), rt.access(pe__, vars__).load()).shiftRight(sa.access().load()));
  }
}