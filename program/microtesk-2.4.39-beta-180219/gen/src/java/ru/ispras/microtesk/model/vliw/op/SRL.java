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

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.vliw.PE;
import ru.ispras.microtesk.model.vliw.TempVars;
import ru.ispras.microtesk.model.vliw.mode.*;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.vliw.TypeDefs.*;

public final class SRL extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "SRL",
          SRL.class,
          null
          );
      addArgument("rd", R.INFO);
      addArgument("rt", R.INFO);
      addArgument("sa", INDEX);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R rd = (R) getArgument("rd", args);
      final R rt = (R) getArgument("rt", args);
      final Immediate sa = (Immediate) getArgument("sa", args);

      return new SRL(rd, rt, sa);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R rd;
  public final R rt;
  public final Immediate sa;

  public SRL(final R rd, final R rt, final Immediate sa) {
    assert R.INFO.isSupported(rd);
    assert R.INFO.isSupported(rt);
    assert INDEX.equals(sa.getType());

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
    return String.format("SRL %s, %s, %d", rd.text(vars__), rt.text(vars__), sa.access().load().bigIntegerValue());
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
    rd.access(pe__, vars__).store(rt.access(pe__, vars__).load().shiftRight(sa.access().load()));
  }
}