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

public final class la extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "la",
          la.class,
          null
          );
      addArgument("rd", REG.INFO);
      addArgument("addr", LA_ADDR.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rd = (REG) getArgument("rd", args);
      final LA_ADDR addr = (LA_ADDR) getArgument("addr", args);

      return new la(rd, addr);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rd;
  public final LA_ADDR addr;

  public la(final REG rd, final LA_ADDR addr) {
    assert REG.INFO.isSupported(rd);
    assert LA_ADDR.INFO.isSupported(addr);

    this.rd = rd;
    this.addr = addr;

    addArgument("rd", rd);
    addArgument("addr", addr);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("la %s, %s", rd.text(vars__), addr.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s%s", new lui(rd, new Immediate(addr.value.access().bitField(31, 16))).image(vars__), new ori(rd, rd, new Immediate(addr.value.access().bitField(15, 0))).image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    new instruction(new lui(rd, new Immediate(addr.value.access().bitField(31, 16)))).execute(pe__, vars__);
    new instruction(new ori(rd, rd, new Immediate(addr.value.access().bitField(15, 0)))).execute(pe__, vars__);
  }
}