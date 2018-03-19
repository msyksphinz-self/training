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

package ru.ispras.microtesk.model.vmem.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.vmem.PE;
import ru.ispras.microtesk.model.vmem.TempVars;
import ru.ispras.microtesk.model.vmem.mode.*;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.vmem.TypeDefs.*;

public final class mh extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "mh",
          mh.class,
          null
          );
      addArgument("rt", REG.INFO);
      addArgument("im", BYTE);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rt = (REG) getArgument("rt", args);
      final Immediate im = (Immediate) getArgument("im", args);

      return new mh(rt, im);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "mh",
          instruction.class,
          null
          );
      addArgument("rt", REG.INFO);
      addArgument("im", BYTE);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rt = (REG) getArgument("rt", args);
      final Immediate im = (Immediate) getArgument("im", args);

      return new instruction(
        new mh(
          rt,
          im
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rt;
  public final Immediate im;

  public mh(final REG rt, final Immediate im) {
    assert REG.INFO.isSupported(rt);
    assert BYTE.equals(im.getType());

    this.rt = rt;
    this.im = im;

    addArgument("rt", rt);
    addArgument("im", im);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("mh %s %x", rt.text(vars__), im.access().load().bigIntegerValue(false));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("0010%s%s", rt.image(vars__), im.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    rt.access(pe__, vars__).bitField(15, 8).store(im.access().load());
  }
}